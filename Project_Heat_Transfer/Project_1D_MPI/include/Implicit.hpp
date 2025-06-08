#ifndef IMPLICIT_HPP
#define IMPLICIT_HPP

#include <vector>
#include <petscksp.h>

void solveImplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out,
                   bool restartMode,
                   int restart_step);

#endif



