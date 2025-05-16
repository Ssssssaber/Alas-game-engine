mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
msbuild ProjectSolution.sln /property:Configuration=Debug
cmake --build . --config Debug
cd ..
xcopy .\EntryPoint\Assets\ .\bin\Debug\Assets\ /e
copy MiscScripts\imgui.ini .\bin\Debug\
copy LICENSE .\bin\Debug\
