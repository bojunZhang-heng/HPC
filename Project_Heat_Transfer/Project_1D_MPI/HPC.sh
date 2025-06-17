#!/bin/bash

rm -rf *.log
rm -rf build
mkdir build
cd build
cmake ../ -DCMAKE_CXX_COMPILER=icpc
make -j

cd ..

if [ "$1" == "fixed_dt" ]; then
    rm -f error_dx.dat
    for N in 8 16 32 64 128; do
        echo "Running with N = $N"
        ./build/HeatTransfer1D $N 1.0 fixed_dt >> run_N${N}.log 2>&1
    done
    echo "Done. Data saved to error_dx.dat"

elif [ "$1" == "fixed_dx" ]; then
    rm -f error_dt.dat
    for CFL in 0.5 0.32 0.16 0.08 0.04; do
        echo "Running with CFL = $CFL"
        ./build/HeatTransfer1D 1.0 $CFL fixed_dx >> run_CFL${CFL}.log 2>&1
    done
    echo "Done. Data saved to error_dt.dat"

else
    echo "Usage: $0 [fixed_dx|fixed_dt]"
    exit 1
fi



