#include <cmath>

bool is_equal(double aa, double bb, double eps = 1e-8) {
    return std::fabs(aa - bb) < eps;
}

