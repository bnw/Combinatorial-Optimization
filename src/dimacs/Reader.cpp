#include <ios>
#include <sstream>
#include <cassert>
#include "Reader.h"

namespace dimacs {

std::unique_ptr<ED::Graph> Reader::read(std::istream &input)
{
	std::unique_ptr<ED::Graph> graph = nullptr;
	while (input) {
		std::string line_buffer;
		std::getline(input, line_buffer);
		std::istringstream line_stream(line_buffer);
		char command;
		line_stream >> std::skipws >> command;
		if (not line_stream) {
			continue;
		}
		switch (command) {
			case 'c': // comment
				break;
			case 'p': // p edge n m
				create_graph(graph, line_stream);
				break;
			case 'e': // e i j
				create_edge(graph, line_stream);
				break;
			default:
				throw "Unknown line start '" + std::string(&command, 1) + "' in input.";
		}
	}
	assert(graph);
	return graph;
}

void Reader::create_graph(std::unique_ptr<ED::Graph> &graph, std::istringstream &line_stream)
{
	assert(not graph);
	std::string type;
	size_t n, m;
	line_stream >> type >> n >> m;
	assert(type == "edge");
	graph = std::unique_ptr<ED::Graph>(new ED::Graph(n));
}

void Reader::create_edge(std::unique_ptr<ED::Graph> const &graph, std::istringstream &line_stream)
{
	auto x = line_stream.str();
	assert(graph);
	ED::NodeId v, w;
	line_stream >> v >> w;
	assert(v > 0);
	assert(w > 0);
	assert(v <= graph->num_nodes());
	assert(w <= graph->num_nodes());
	graph->add_edge(v - 1, w - 1);
}

}
