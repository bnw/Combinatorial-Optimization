# Combinatorial-Optimization
Combinatorial Optimization WS 2016/2017 
http://www.or.uni-bonn.de/lectures/ws16/cows16.html   
Implementation of a cardinality matching algorithm with runtime O(n m logn)

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

Graph must be provided in DIMACS format.

Tests
-----
Some unit tests using the Boost testing framework are provided.
Run via: 

`./tests`
