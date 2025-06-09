//--------------------------------------------------------------------------
// u,t = nu*u,xx + g
// u   = g       at x = 0
// u,x = h/kappa at x = N
//--------------------------------------------------------------------------
#include "Explicit.hpp"
#include "Implicit.hpp"
#include "Analytical.hpp"
#include "ErrorEstimator.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <petscsys.h>

int main(int argc, char **argv)
{
  PetscInitialize(&argc, &argv, NULL, NULL);

//------------------------------------------
// Physcial Variable 
//*
   double kappa = 1.0;
   double rou   = 10.0;
   double c     = 1.0;
   double nu    = kappa / (rou * c);
   double u0    = 1.0;

//------------------------------------------
// Spatial Discretization
//*
   int    N     = 16; 
   double CFL   = 0.32;
   double aL    = 1.0;

   std::string mode = "fixed_dx";

   if (argc >= 3) {
     if (std::string(argv[3]) == "fixed_dx") {
       aL  = std::stod(argv[1]);
       CFL = std::stod(argv[2]);
     }
     else if (std::string(argv[3]) == "fixed_dt") {
       N   = std::stoi(argv[1]);
       CFL = std::stod(argv[2]);
     }
     mode = argv[3];
   }

   double  dx  = aL / N;
   double  g_x = 2.0 * u0 * nu / (aL * aL);

//------------------------------------------
// Temporal Discretization
//*
    
   double  dt     = CFL * dx * dx / nu;
   double  Tend   = 5.0 * aL * aL / nu;
   int     Nsteps = static_cast<int>(Tend / dt);
   std::cout << "CFL = " << CFL << '\n';
   std::cout << "dt = "  << dt  << '\n';
   std::cout << "Nsteps = " << Nsteps << '\n';


//------------------------------------------
// Initial Contion
//*
  std::vector<double> u_E(N+1, 1.0);
  std::vector<double> u_I(N+1, 1.0);
  std::vector<double> u_A(N+1, 1.0);



//------------------------------------------
// Boundary Condition
//*
  double hh   = 1.0;
  double Diri = 1.0;
  double Neu  = hh / kappa;

//------------------------------------------
// HDF5 Setting 
//*
  bool restart    = false; 
  int  start_step = 20;
  if (argc > 1 && std::string(argv[1]) == "--restart") {
    restart = true;
  }


//------------------------------------------
// Numerical solution Explicit Euler Scheme
//*
  solveAnalytical(N, dx, u_A);
  solveExplicit(N, dt, dx, Neu, Diri, Nsteps, CFL, g_x, u_E, restart, start_step);
  solveImplicit(N, dt, dx, Neu, Diri, Nsteps, CFL, g_x, u_I, restart, start_step);


//------------------------------------------
// Print solution
//*
  std::cout << "Solution" << '\n';
  std::cout << std::left
            << std::setw(12) << "Explicit"
            << std::setw(12) << "Implicit"
            << std::setw(12) << "Analitical" << '\n';

  for (int ii = 0; ii <= N; ++ii) {
      std::cout << std::fixed << std::setprecision(5)
                << std::setw(12) << u_E[ii]
                << std::setw(12) << u_I[ii]
                << std::setw(12) << u_A[ii]
                << '\n';
  }

//------------------------------------------
// Error Evaluation
//*
  double err_E = computeMaxError(u_E, u_A);
  double err_I = computeMaxError(u_I, u_A);

  writeErrorDX(dx, err_E);
//  writeErrorDX(dx, err_I);
 
  writeErrorDT(dt, err_E);
//  writeErrorDT(dt, err_I);


  PetscFinalize();
  return 0;
}
//-------------------------------------------------------------
// Varibale  description
// *
// gx                  = f / (rou * c)  
// CFL                   = Numerical stability coeff 
// *
// Spactial----------------------------------
// N                     =  Spatial grid 
// L                     =  Length of spactial domain
// u0                         =  Initial velocity
// nu                         =  kappa/(Rou * c)
// hh                         =  Neumann boundary value
