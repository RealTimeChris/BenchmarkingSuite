#!/bin/bash
"/usr/bin/cmake" -S ./ -B ./Build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/clang++-18
"/usr/bin/cmake" --build ./Build --config=Release