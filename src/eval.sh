#!/bin/bash
set -euo pipefail
rm -rf build

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER="clang++-18" -DCMAKE_C_COMPILER="clang-18" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --target tests


FAILED=0
build/tests || { FAILED=1; echo "FAILED: tests failed"; };
if [ $FAILED -eq 0 ]; then
    echo "All tests passed"
fi
exit $FAILED

