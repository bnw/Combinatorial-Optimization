#include <iostream>
#include <fstream>
#include <graph.hpp>
#include <utils.h>
#include <cassert>
#include <memory>
#include <dimacs/Reader.h>
#include <dimacs/Writer.h>
#include "Matching.h"
#include "AlternatingTree.h"
#include "ShrinkableGraph.h"
#include "utils.h"
#include "OddCircuit.h"
#include "algorithms/CardinalityMatchingAlgorithm.h"
#include "MatchingFactory.h"

void print_usage(const char *argv[])
{
	std::cout << "Usage: " << argv[0] << " --graph file1.dmx [--hint file2.dmx]" << std::endl;
}

int main(int argc, const char *argv[])
{
	try {
		algorithms::CardinalityMatchingAlgorithm const algorithm{};
		MatchingFactory const matching_factory{};
		dimacs::Reader const reader{};
		dimacs::Writer const writer{};
		switch (argc) {
			case 3 : {
				std::ifstream file(argv[2]);
				auto G = reader.read(file);
				auto const M = algorithm.run(*G);
				writer.write(*G, M, std::cout);
				break;
			}
			case 5: {
				std::ifstream graph_file(argv[2]);
				std::ifstream hint_matching_file(argv[4]);
				auto G = reader.read(graph_file);
				auto const hint_matching_graph = reader.read(hint_matching_file);
				auto hint_matching = matching_factory.create_from_subgraph(*G, *hint_matching_graph);
				auto const M = algorithm.run(*G, hint_matching);
				writer.write(*G, M, std::cout);
				break;
			}
			default:
				print_usage(argv);
		}
	} catch (std::exception const &exception) {
		std::cout << "Exception: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	} catch (...) {
		std::cout << "Unknown exception" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}