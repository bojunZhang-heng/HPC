#ifndef HDF5_WRITER_HPP
#define HDF5_WRITER_HPP

#include <string>
#include <vector>
#include <H5Cpp.h>

namespace HDF5 {

class Writer {
public:
    Writer(const std::string& filename);
    ~Writer();

    void write_snapshot(const std::vector<double>& data, int step);

private:
    H5::H5File file;
};

}

#endif 
