all:
	g++ -std=c++11 -Isrc/ -Ilib/ -O3 -Wall -o task1 lib/graph.cpp src/AlternatingTree.cpp src/Edge.cpp src/Matching.cpp src/MatchingFactory.cpp src/OddCircuit.cpp src/ShrinkableGraph.cpp src/UnionFind.cpp src/algorithms/CardinalityMatchingAlgorithm.cpp src/algorithms/MatchingUnshrinkAlgorithm.cpp src/dimacs/Reader.cpp src/dimacs/Writer.cpp src/task1.cpp
