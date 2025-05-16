#!/bin/bash

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..
copy -r EntryPoint/Assets bin/Release/
copy MiscScripts/imgui.ini /bin/Release/
copy LICENSE bin/Release/
