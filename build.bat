mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
msbuild ProjectSolution.sln /property:Configuration=Release
cmake --build . --config Release
cd ..
xcopy .\EntryPoint\Assets\ .\bin\Release\Assets\ /e
copy MiscScripts\imgui.ini .\bin\Release\
copy LICENSE .\bin\Release\
