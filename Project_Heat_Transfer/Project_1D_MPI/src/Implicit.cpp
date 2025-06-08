#include "Implicit.hpp"
#include "HDF5_Writer.hpp"
#include "HDF5_Reader.hpp"
#include <H5Cpp.h>
#include <string>
#include <vector>
#include <petscksp.h>
#include <iostream>

void solveImplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out,
                   bool restartMode,
                   int restart_step)
{
  PetscInt size = N-1; 
  Vec u, b;
  Mat A;
  KSP ksp;

  PetscScalar diag = 1.0 + 2.0 * CFL;
  PetscScalar offd = -CFL;

  VecCreate(PETSC_COMM_WORLD, &u);
  VecSetSizes(u, PETSC_DECIDE, size);
  VecSetFromOptions(u);
  VecDuplicate(u, &b);

  MatCreate(PETSC_COMM_WORLD, &A);
  MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, size, size);
  MatSetFromOptions(A);
  MatSetUp(A);

  for (int ii = 0; ii<size; ++ii) {
    if (ii > 0) {
      MatSetValue(A, ii, ii - 1, offd, INSERT_VALUES);
    }

    MatSetValue(A, ii, ii, diag, INSERT_VALUES);
    
    if (ii < size-1) {
      MatSetValue(A, ii, ii+1, offd, INSERT_VALUES);
    }     
  }
  PetscScalar neu_val = diag + offd;
  MatSetValue(A, size-1, size-1, neu_val, INSERT_VALUES);

  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);

  KSPCreate(PETSC_COMM_WORLD, &ksp);
  KSPSetOperators(ksp, A, A);
  KSPSetFromOptions(ksp);

  VecSet(u, 1.0 + g_x*dt);
  PetscInt    Diri_indices[1] = {0};
  PetscScalar Diri_contrib[1] = {-offd * Diri};

  PetscInt    Neu_indices[1] = {size-1};
  PetscScalar Neu_contrib[1] = {-offd * Neu * dx};

  std::vector<double> u_std(N+1, 1.0 + g_x*dt);
  int start_step = 0;
  if (restartMode) {
    try {
      HDF5::Reader reader("Solution_Implicit.h5");
      reader.read_snapshot(u_std, restart_step);
      std::cout << "[Implicit] Restart from step " << restart_step << '\n';
      start_step = restart_step + 1;
    } catch (const std::exception& e) {
      std::cerr << "[Implicit] Failed to read snapshot: " << e.what() << '\n';
      return;
    }
  }
  // === Set Vec u ===
  for (int ii = 1; ii < N; ++ii) {
    VecSetValue(u, ii - 1, u_std[ii], INSERT_VALUES);
  }

  VecAssemblyBegin(u);
  VecAssemblyEnd(u);

  HDF5::Writer writer("Solution_Implicit.h5");

  for (int step = 0; step < Nsteps; ++step) {
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

    for (int ii=1; ii<N; ++ii){
      u_std[ii] = u_array[ii-1];
    }
    u_std[0] = Diri;
    u_std[N] = u_std[N-1] + Neu*dx;

    VecRestoreArrayRead(u, &u_array);

    if (step % 10 == 0) {
      writer.write_snapshot(u_std, step);
    }

  }

  u_out = u_std;

  VecDestroy(&u); 
  VecDestroy(&b); 
  MatDestroy(&A);
  KSPDestroy(&ksp);

}

