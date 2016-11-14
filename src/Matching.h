#ifndef COMBINATORIAL_OPTIMIZATION_MATCHING_H
#define COMBINATORIAL_OPTIMIZATION_MATCHING_H

#include <graph.hpp>
#include <cassert>
#include "Edge.h"
#include "typedefs.h"

class Matching
{
public:
	Matching(ED::Graph const &graph) :
			covering_edges(graph.num_nodes(), Edge::invalid_edge()),
			covered_nodes(graph.num_nodes(), false)
	{}

	void add_edge(Edge const &edge)
	{
		for (auto const node_id : edge.get_node_ids()) {
			assert(not is_covered(node_id));
			set_covered(node_id, true);
			covering_edges.at(node_id) = edge;
		}
	}

	void set_covered(NodeId const node_id, bool const covered)
	{
		covered_nodes.at(node_id) = covered;
	}

	bool is_covered(NodeId const node_id) const
	{
		return covered_nodes.at(node_id);
	}

private:
	std::vector<Edge> covering_edges;
	std::vector<bool> covered_nodes;
};


#endif //COMBINATORIAL_OPTIMIZATION_MATCHING_H
