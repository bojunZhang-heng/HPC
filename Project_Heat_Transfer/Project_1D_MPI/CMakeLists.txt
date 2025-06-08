cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0074 NEW)
project(HeatTransfer1D CXX)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================================
# Find MPI
# ==================================
find_package(MPI REQUIRED CXX)
include_directories(${MPI_INCLUDE_PATH})
SET(CMAKE_CXX_COMPILER ${MPI_CXX_COMPLIER})

# ==================================
# Find PETSc
# ==================================
SET(PETSC_DIR "/work/mae-liuj/lib/petsc-3.16.6-opt")
include_directories(${PETSC_DIR}/include)
find_path(PETSC_INCLUDE_DIR NAMES petsc.h PATHS ${PETSC_DIR}/include)
find_library(PETSC_LIBRARY NAMES petsc PATHS ${PETSC_DIR}/lib)

include_directories(${PETSC_INCLUDE_DIR})
link_directories(${PETSC_DIR}/lib)

# ==================================
# Find HDF5
# ==================================
set(HDF5_DIR "/share/base/hdf5/1.10.4-intel-18.4")
include_directories(${HDF5_DIR}/include)
link_directories(${HDF5_DIR}/lib)
file(GLOB HDF5_LIBS ${HDF5_DIR}/lib/lib*.so)

# ==================================
# Include source & headers
# ==================================
SET(SRC_FILES
    src/driver.cpp
    src/Analytical.cpp
    src/Explicit.cpp
    src/Implicit.cpp
    src/HDF5_Writer.cpp
    src/HDF5_Reader.cpp
    src/ErrorEstimator.cpp
)
include_directories(${PROJECT_SOURCE_DIR}/include)

# ==================================
# Executable
# ==================================
add_executable(HeatTransfer1D ${SRC_FILES})

  
# ==================================
# Complier Flags
# ==================================
target_compile_options(HeatTransfer1D PRIVATE -Wall -O2)

# ==================================
# Link Libraries
# ==================================
target_link_libraries(HeatTransfer1D
    ${HDF5_LIBS} 
    petsc
    mpi
    mpicxx
    stdc++
    m
    dl
)

