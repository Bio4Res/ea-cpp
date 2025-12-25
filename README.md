# ea-cpp 

A C++ port from https://github.com/Bio4Res/ea that aims at experimenting with modern C++ techniques to reduce energy compsumption.

It has been tested with Visual Studio 2019/22, gcc14 and clang18 with -std=c++23.

Dependencies:

nlohmann.json 3.11.3 (a header-only library to manage JSON files)

Build:

g++ -O3 -march=native -DNDEBUG -Wall -I. -o ea_main *.cpp -std=c++23 
