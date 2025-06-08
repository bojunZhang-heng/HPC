#include "Explicit.hpp"
#include "HDF5_Writer.hpp"
#include "HDF5_Reader.hpp"
#include <H5Cpp.h>
#include <string>
#include <vector>
#include <iostream>

void solveExplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out, 
                   bool restartMode,
                   int restart_step) 
{
  std::vector<double> u(N+1, 1.0), u_old(N+1, 1.0);
  int start_step = 0;

  if (restartMode) {
    try {
      HDF5::Reader reader("Solution_Explicit.h5");
      reader.read_snapshot(u, restart_step);
      std::cout << "[Explicit] Restart from step " << restart_step << '\n';

      u_old = u;
      start_step = start_step + 1;
    } catch (const std::exception& e) {
      std::cerr << "[Explicit] Failed to read snapshot: " << e.what() << '\n';
      return;
    }
  }

  HDF5::Writer  writer("Solution_Explicit.h5");

  for (int it = 0; it < Nsteps; ++it) {
    for (int jj = 1; jj < N; ++jj) {
      u[jj] = u_old[jj] + CFL * (u_old[jj-1] - 2*u_old[jj] + u_old[jj+1]) + dt*g_x;
    }
//------------------------------------
// Apply Boundayr Condition
//*
    u[0] = Diri;
    u[N] = u[N-1] + Neu * dx;
    u_old = u;

//------------------------------------
// Store the solution every 10 iterations
//*
    if (it % 10 == 0) {
      writer.write_snapshot(u, it);
    }

    u_out = u;
  }

}
