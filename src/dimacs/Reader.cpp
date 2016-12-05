#include <ios>
#include <sstream>
#include <cassert>
#include "Reader.h"

namespace dimacs {

std::unique_ptr<ShrinkableGraph> Reader::read(std::istream &input) const
{
	std::unique_ptr<ShrinkableGraph> graph = nullptr;
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
	if(not graph){
		throw std::runtime_error("Error while reading graph");
	}
	return graph;
}

void Reader::create_graph(std::unique_ptr<ShrinkableGraph> &graph, std::istringstream &line_stream) const
{
	std::string type;
	size_t n, m;
	line_stream >> type >> n >> m;
	assert(type == "edge");
	graph = std::unique_ptr<ShrinkableGraph>(new ShrinkableGraph(n));
}

void Reader::create_edge(std::unique_ptr<ShrinkableGraph> const &graph, std::istringstream &line_stream) const
{
	auto x = line_stream.str();
	ED::NodeId v, w;
	line_stream >> v >> w;
	assert(v > 0);
	assert(w > 0);
	assert(v <= graph->num_nodes());
	assert(w <= graph->num_nodes());
	graph->add_edge(ED::dimacs_id_to_ED_id(v), ED::dimacs_id_to_ED_id(w));
}

}
