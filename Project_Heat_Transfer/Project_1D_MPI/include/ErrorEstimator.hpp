#ifndef ERRORESTIMATOR_HPP
#define ERRORESTIMATOR_HPP

#include <vector>

double computeMaxError(const std::vector<double>& u_num, const std::vector<double>& u_exact);

void writeErrorDX(double dx, double error);

void writeErrorDT(double dt, double error);

#endif

