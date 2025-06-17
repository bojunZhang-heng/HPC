#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

void write_txt(const std::vector<double>& u, double dx, const std::string& filename) {
    std::ofstream fout(filename);
    for (size_t ii = 0; ii < u.size(); ++ii) {
        fout << ii * dx << " " << u[ii] << "\n";
    }
    std::cout << "Output successfully" << '\n';
    fout.close();
}

