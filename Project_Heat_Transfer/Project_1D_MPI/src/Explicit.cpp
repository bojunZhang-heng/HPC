#include "Explicit.hpp"
#include "HDF5_Writer.hpp"
#include "HDF5_Reader.hpp"
#include "is_equal.hpp"
#include "write_txt.hpp"
#include <H5Cpp.h>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

void solveExplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out, 
                   bool restartMode,
                   int restart_step) 
{
  std::vector<double> u(N+1, 1.0), u_old(N+1, 1.0);
  std::vector<double> dudx(N+2, 1.0);
  std::string Output_Dir = "./postprocess/";
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

  double tt = 0.0;
  double t_dimensionless = 0.0;

  std::ofstream fout_dudx("./postprocess/dudx_t.txt");

  for (int it = 0; it < 100000; ++it) {
    tt = it * dt;
    t_dimensionless = tt;
//    std::cout << "tt == "  << tt << '\n';
    for (int jj = 1; jj < N; ++jj) {
      u[jj] = u_old[jj] + CFL * (u_old[jj-1] - 2*u_old[jj] + u_old[jj+1]) + dt*g_x;
//------------------------------------
// Apply Boundayr Condition
//*
      u[0] = Diri;
      u[N] = u[N-1] + Neu * dx;
      u_old = u;
    }
//------------------------------------
// Store the solution every 10 iterations
//*
    if (it % 10 == 0) {
      writer.write_snapshot(u, it);
    }

//------------------------------------
// Output u at different time
//*
    if (is_equal(t_dimensionless, 0.0)){
      std::cout << "call tt = 0" << "\n";
      write_txt(u, dx, "./postprocess/outputT=000.txt");

    } 
    else if (is_equal(t_dimensionless, 0.25*5.0)) {
      std::cout << "call tt = 0.25T" << "\n";
      write_txt(u, dx, "./postprocess/outputT=025.txt");
    } 
    else if (is_equal(t_dimensionless, 0.5*5.0)) {
      write_txt(u, dx, "./postprocess/outputT=050.txt");
    }
    else if (is_equal(t_dimensionless, 0.75*5.0)) {
      write_txt(u, dx, "./postprocess/outputT=075.txt");
    }
    else if (is_equal(t_dimensionless, 1*5.0)) {
      write_txt(u, dx, "./postprocess/outputT=100.txt");
    }

//------------------------------------
// Find the dudx and record at every time step 
//*
    for (int ii = 1; ii<=N-1; ++ii) {
      dudx[ii] = (u[ii+1] - u[ii-1]) / (2*dx);
    }
 
    for (int ii = 1; ii<=N-1; ++ii) {
      fout_dudx << ii*dx << " " <<  tt  << " " << dudx[ii] << '\n';
    }

  }
  fout_dudx.close();
  u_out = u;
}





