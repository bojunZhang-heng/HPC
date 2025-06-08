#ifndef EXPLICIT_HPP
#define EXPLICIT_HPP

#include<vector>

void solveExplicit(int N, double dt, double dx, double Neu, double Diri,
                   int Nsteps, double CFL, double g_x,
                   std::vector<double>& u_out, 
                   bool restartMode,
                   int restart_step);

#endif
