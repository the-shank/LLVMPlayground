#!/bin/bash

set -eu
set -o pipefail

# build
(
  cd build
  # rm -rf ./*
  # cmake -DUSE_REFERENCE=OFF -DCMAKE_BUILD_TYPE=Debug ..
  # ! make clean
  make -j$(nproc)
)

echo ""
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo ""

# run
(
  cd DivZero/test
  opt -load ../../build/DivZero/libDataflowPass.so -DivZero ${1}.opt.ll
  # opt -load ../../build/DivZero/libDataflowPass.so -DivZero -disable-output ${1}.opt.ll
  # opt -load ../../build/DivZero/libDataflowPass.so -DivZero simple1.opt.ll
)
