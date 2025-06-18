#include "Implicit.hpp"
#include "HDF5_Writer.hpp"
#include "HDF5_Reader.hpp"
#include <H5Cpp.h>
#include <string>
#include <vector>
#include <petscksp.h>
#include <iostream>
#include <mpi.h>

void solveImplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out,
                   bool restartMode,
                   int restart_step)
{
  MPI_Comm comm = PETSC_COMM_WORLD;

  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);


  PetscInt global_size = N-1; 
  Vec u, b;
  Mat A;
  KSP ksp;

  PetscScalar diag = 1.0 + 2.0 * CFL;
  PetscScalar offd = -CFL;

// Create Vectors
  VecCreate(comm, &u);
  VecSetSizes(u, PETSC_DECIDE, global_size);
  VecSetFromOptions(u);
  VecDuplicate(u, &b);

// Create matrix 
  MatCreateAIJ(comm, PETSC_DECIDE, PETSC_COMM_WORLD,
               global_size, global_size, 
               3, NULL, 1, NULL, &A);

// Determine ownership range
  PetscInt Istart, Iend;
  MatGetOwnershipRange(A, &Istart, &Iend);

// === Timeing Start ====
  PetscLogDouble t_start, t_asm_end, t_solve_end;
  PetscTime(&t_start);

  for (PetscInt ii = Istart; ii<Iend; ++ii) {
    if (ii > 0) {
      MatSetValue(A, ii, ii - 1, offd, INSERT_VALUES);
    }

    MatSetValue(A, ii, ii, diag, INSERT_VALUES);
    
    if (ii < global_size-1) {
      MatSetValue(A, ii, ii+1, offd, INSERT_VALUES);
    }     
  }

// Neumann BC for last point
  if (Iend == global_size) {
    PetscScalar neu_val = diag + offd;
    MatSetValue(A, global_size-1, global_size-1, neu_val, INSERT_VALUES);
  }

  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
  PetscTime(&t_asm_end);

  KSPCreate(comm, &ksp);
  KSPSetOperators(ksp, A, A);
  KSPSetFromOptions(ksp);

  VecSet(u, 1.0 + g_x*dt);

// Dirichlet BC 
  PetscInt    Diri_indices[1] = {0};
  PetscScalar Diri_contrib[1] = {-offd * Diri};

// Neumann BC 
  PetscInt    Neu_indices[1] = {global_size-1};
  PetscScalar Neu_contrib[1] = {-offd * Neu * dx};

  std::vector<double> u_std(N+1, 1.0 + g_x*dt);
  int start_step = 0;

  if (restartMode) {
    try {
      HDF5::Reader reader("Solution_Implicit.h5");
      reader.read_snapshot(u_std, restart_step);

      if (rank == 0) {
        std::cout << "[Implicit] Restart from step " << restart_step << '\n';
      }

      start_step = restart_step + 1;
    } catch (const std::exception& e) {
      if (rank == 0) {
        std::cerr << "[Implicit] Failed to read snapshot: " << e.what() << '\n';
      }
      return;
    }
  }

  // === Set Vec u ===
  
  VecGetOwnershipRange(u, &Istart, &Iend);
  for (int ii = Istart; ii < Iend; ++ii) {
    VecSetValue(u, ii, u_std[ii+1], INSERT_VALUES);
  }

  VecAssemblyBegin(u);
  VecAssemblyEnd(u);

  HDF5::Writer writer("Solution_Implicit.h5");

  for (int step = start_step; step < 5000; ++step) {
    VecCopy(u, b);
    VecShift(b, g_x*dt);

    VecAssemblyBegin(b);
    VecAssemblyEnd(b);
  
    VecSetValues(b, 1, Diri_indices, Diri_contrib, ADD_VALUES);
    VecSetValues(b, 1, Neu_indices,  Neu_contrib,  ADD_VALUES);

    VecAssemblyBegin(b);
    VecAssemblyEnd(b);

      
    KSPSolve(ksp, b, u);

    const PetscScalar *u_array;
    VecGetArrayRead(u, &u_array);

    for (PetscInt ii=Istart; ii<Iend ++ii){
      u_std[ii+1] = u_array[ii - Istart];
    }

    VecRestoreArrayRead(u, &u_array);

    u_std[0] = Diri;
    u_std[N] = u_std[N-1] + Neu*dx;


    if (step % 10 == 0) {
      writer.write_snapshot(u_std, step);
    }
  }

  PetscTime(&t_solve_end);

  
  if (rank == 0) {
    double asm_time = t_asm_end - t_start;
    double solve_time = t_solve_end - t_asm_end;
    double total_time = t_solve_end - t_start;
    std::cout << "[Timer] Assembly: " << asm_time << "s\n";
    std::cout << "[Timer] Solve:    " << solve_time << "s\n";
    std::cout << "[Timer] Total:    " << total_time << "s\n";
  }

  u_out = u_std;

  VecDestroy(&u); 
  VecDestroy(&b); 
  MatDestroy(&A);
  KSPDestroy(&ksp);

}

