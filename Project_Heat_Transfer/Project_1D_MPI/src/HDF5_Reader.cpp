#include "HDF5_Reader.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace HDF5 {

Reader::Reader(const std::string& filename)
    : file(filename, H5F_ACC_RDONLY) 
{}

Reader::~Reader() {
    file.close();
}

void Reader::read_snapshot(std::vector<double>& data, int step) {
    std::ostringstream group_name;
    group_name << "/Snapshots/Step_" << std::setw(4) << std::setfill('0') << step;

    if (!H5Lexists(file.getId(), group_name.str().c_str(), H5P_DEFAULT)) {
        throw std::runtime_error("Snapshot group not found: " + group_name.str());
    }

    H5::Group group = file.openGroup(group_name.str());
    H5::DataSet dataset = group.openDataSet("values");

    H5::DataSpace dataspace = dataset.getSpace();
    hsize_t dims[1];
    dataspace.getSimpleExtentDims(dims, nullptr);

    data.resize(dims[0]);
    dataset.read(data.data(), H5::PredType::NATIVE_DOUBLE);
}

}

