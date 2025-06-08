#ifndef HDF5_READER_HPP
#define HDF5_READER_HPP

#include <string>
#include <vector>
#include <H5Cpp.h>

namespace HDF5 {

class Reader {
public:
    Reader(const std::string& filename);
    ~Reader();

    void read_snapshot(std::vector<double>& data, int step);

private:
    H5::H5File file;
};

}

#endif 

