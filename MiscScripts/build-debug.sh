#!/bin/bash

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
cd ..
copy -r EntryPoint/Assets bin/Debug/
copy MiscScripts/imgui.ini /bin/Debug/
copy LICENSE bin/Debug/
