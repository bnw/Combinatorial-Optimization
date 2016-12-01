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


	struct Instance
	{
		std::string name;
		std::size_t max_cardinality;
	};

	std::vector<Instance> instances = {
//			{"ar9152",   4349},
//			{"ch71009",        35025},
//			{"ei8246",         4123},
			{"fixed",      500},
//			{"gr9882",         4931},
			{"K2",         1},
			{"K3",         1},
			{"K4",         2},
			{"lu980",      490},
			{"P3",         2},
			{"pbd984",     492},
			{"peterson",   5},
			{"pma343",     171},
			{"queen10_10", 50},
			{"queen11_11", 60},
			{"queen16_16", 128},
			{"queen27_27", 364},
			{"queen4_4",   8},
			{"queen5_5",   12},
			{"queen6_6",   18},
			{"queen7_7",   24},
			{"queen8_8",   32},
			{"queen9_9",   40},
			{"simple",     2},
//			{"USA-road-d.FLA", 507967},
//		{"USA-road-d.USA", 0},
			{"xqf131",     64}
	};


	for (auto const &instance : instances) {
		dimacs::Reader const reader{};
		std::ifstream file("instances\\" + instance.name + ".dmx");
		auto G = reader.read(file);
		auto const M = CardinalityMatchingAlgorithm().run(*G);
		std::cout << instance.name << ": " << M.get_num_edges() << std::endl;
		assert(instance.max_cardinality == M.get_num_edges());
	}

//	dimacs::Reader const reader{};
//	std::ifstream file(argv[1]);
//	auto G = reader.read(file);
//	auto const M = CardinalityMatchingAlgorithm().run(*G);
//	std::cout << argv[1] << ": " << M.get_num_edges() << std::endl;


	return EXIT_SUCCESS;
}