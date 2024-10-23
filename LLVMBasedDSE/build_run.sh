#!/bin/bash

set -e

cd ~/DynamicSymbolicExecution/LLVMBasedDSE
cd build
cmake ..
make 
# export library path?

# input is path to C program
cd ~/DynamicSymbolicExecution/LLVMBasedDSE/DSE
cd test
clang -emit-llvm -S -fno-discard-value-names -c $1.c
opt -load ../../build/DSE/libInstrumentPass.so -Instrument -S $1.ll -o $1.instrumented.ll
../../build/DSE/dse ./$1 $2
timeout $3 ../../build/DSE/dse ./$1
