#!/bin/bash

set -eu
set -o pipefail

# build
(
  cd build
  # rm -rf ./*
  # cmake -DUSE_REFERENCE=OFF -DCMAKE_BUILD_TYPE=Debug ..
  # ! make clean
  # cmake ..
  # ! make clean
  make -j$(nproc)
)

echo ""
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo ""

# run
(
  cd DivZeroInstrument/test
  clang -emit-llvm -S -fno-discard-value-names -c -o ${1}.ll ${1}.c -g
  opt -load ../../build/DivZeroInstrument/libInstrumentPass.so -Instrument -S ${1}.ll -o ${1}.instrumented.ll
  clang -o ${1} -L../../build/DivZeroInstrument -lruntime ${1}.instrumented.ll
  export LD_LIBRARY_PATH=$(realpath ../../build/DivZeroInstrument)
  ./${1} ${@:1}}
  # opt -load ../../build/DivZero/libDataflowPass.so -DivZero -disable-output ${1}.opt.ll
  # opt -load ../../build/DivZero/libDataflowPass.so -DivZero simple1.opt.ll
)
