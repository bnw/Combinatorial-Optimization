# Combinatorial-Optimization
Combinatorial Optimization WS 2016/2017 

Compile with cmake (cool)
-------------------------
```
mkdir build
cd build
cmake ..
make
```

Compile without cmake (not so cool)
-----------------------------------
If cmake is not available, a simple makefile is provided as backup.
This makefile does not allow the compilation of the tests.  
To compile the main program using this makefile simply run:

`make` 

Usage
-----
`./task1 --graph file1.dmx [--hint file2.dmx]`

Tests
-----
Some unit tests using the Boost testing framework are provided.
Run via: 

`./tests`