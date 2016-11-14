#ifndef COMBINATORIAL_OPTIMIZATION_EDGE_H
#define COMBINATORIAL_OPTIMIZATION_EDGE_H

#include <vector>
#include <graph.hpp>
#include <utility>
#include "typedefs.h"

class Edge
{
public:
	using Vector = std::vector<Edge>;

	Edge(NodeId const first_node_id, NodeId const second_node_id) :
			node_ids({first_node_id, second_node_id})
	{}

	static Edge invalid_edge()
	{
		return {ED::invalid_node_id, ED::invalid_node_id};
	}

	bool is_invalid_edge() const
	{
		return first_node_id() == ED::invalid_node_id or second_node_id() == ED::invalid_node_id;
	}

	std::vector<NodeId> get_node_ids() const
	{
		return {first_node_id(), second_node_id()};
	}

	NodeId first_node_id() const
	{
		return node_ids.first;
	}

	NodeId second_node_id() const
	{
		return node_ids.second;
	}

private:
	std::pair<NodeId, NodeId> node_ids;
};


#endif //COMBINATORIAL_OPTIMIZATION_EDGE_H
