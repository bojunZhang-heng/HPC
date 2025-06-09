#include "ErrorEstimator.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>

double computeMaxError(const std::vector<double>& u_num, const std::vector<double>& u_exact) {
    double max_err = 0.0;
    for (size_t ii = 0; ii < u_num.size(); ++ii) {
        double err = std::abs(u_num[ii] - u_exact[ii]);
        if (err > max_err) {
            max_err = err;
        }
    }
    return max_err;
}

void writeErrorDX(double dx, double error) {
    std::ofstream fout("../postprocess/error_dt.dat", std::ios::app);
    fout << std::scientific << std::setprecision(8) << dx << " " << error << "\n";
}

void writeErrorDT(double dt, double error) {
    std::ofstream fout("../postprocess/error_dt.dat", std::ios::app);
    fout << std::scientific << std::setprecision(8) << dt << " " << error << "\n";
}

