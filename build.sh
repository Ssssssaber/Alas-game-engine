#!/bin/bash

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..
cp -r EntryPoint/Assets bin/Release/
cp MiscScripts/imgui.ini bin/Release/
cp LICENSE bin/Release/
