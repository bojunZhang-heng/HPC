#include "HDF5_Writer.hpp"
#include <sstream>
#include <iomanip>

namespace HDF5 {

Writer::Writer(const std::string& filename)
    : file(filename, H5F_ACC_TRUNC) 
{
   
}

Writer::~Writer() {
    file.close();
}

void Writer::write_snapshot(const std::vector<double>& data, int step) {
    std::ostringstream group_name;
    
    if (!H5Lexists(file.getId(), "/Snapshots", H5P_DEFAULT)) {
      file.createGroup("/Snapshots");
    }

    group_name << "/Snapshots/Step_" << std::setw(4) << std::setfill('0') << step;
    
    H5::Group group = file.createGroup(group_name.str());

    hsize_t dims[1] = { data.size() };
    H5::DataSpace dataspace(1, dims);
    H5::DataSet dataset = group.createDataSet("values", H5::PredType::NATIVE_DOUBLE, dataspace);
    dataset.write(data.data(), H5::PredType::NATIVE_DOUBLE);
}

}

