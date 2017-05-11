#bin/bash

export LD_LIBRARY_PATH=$(pwd)
make clean
make
make fmapp
./fmalgorithm > report.txt