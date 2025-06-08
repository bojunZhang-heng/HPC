#include "Analytical.hpp"
#include <vector>
#include <cmath>

void solveAnalytical(int N, double dx, std::vector<double>& u_out)
{
  std::vector<double> u(N+1, 1.0);
  std::vector<double> x_coor(N+1, 0.0);

  for (int jj=0; jj<N+1; ++jj ){
    x_coor[jj] = dx * jj;
    u[jj]      = -pow(x_coor[jj], 2.0) + 3*x_coor[jj] + 1.0;
  }

  u_out = u;

}

