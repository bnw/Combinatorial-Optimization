#include <iostream>
#include <fstream>
#include <graph.hpp>
#include <numeric>
#include <utils.h>
#include <set>
#include <cassert>
#include <memory>
#include <dimacs/Reader.h>
#include "Matching.h"
#include "AlternatingTree.h"
#include "ShrinkableGraph.h"
#include "utils.h"
#include "UnevenCircuit.h"
#include "CardinalityMatchingAlgorithm.h"


int main(int argc, const char *argv[])
{

	dimacs::Reader const reader{};
	std::ifstream file(argv[1]);
	auto G = reader.read(file);
	auto const M = CardinalityMatchingAlgorithm().run(*G);
//	dead_nodes.push_back(T.get_root_node_id());

	std::cout << "Cardinality of maximum matching is " << M.get_num_edges() << std::endl;


	return EXIT_SUCCESS;
}