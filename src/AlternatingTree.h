#ifndef COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H
#define COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H

#include <cassert>
#include "ShrinkableGraph.h"

class AlternatingTree
{
private:
	enum NodeType
	{
		ODD,
		EVEN,
		NOT_CONTAINED
	};
public:
	AlternatingTree(ShrinkableGraph const &graph, NodeId const node_id) :
			G(graph),
			node_types(G.num_nodes(), NOT_CONTAINED)
	{
		node_types.at(node_id) = EVEN;
	}

	void add_edge(Edge const &edge)
	{
		assert(not contains(edge.first_node_id()) or not contains(edge.second_node_id()));

		//Find out which endpoint is already in the alternating tree and which is new
		NodeId contained_node_id;
		NodeId new_node_id;
		if (not contains(edge.first_node_id())) {
			new_node_id = edge.first_node_id();
			contained_node_id = edge.second_node_id();
		} else {
			assert(not contains(edge.second_node_id()));
			new_node_id = edge.second_node_id();
			contained_node_id = edge.first_node_id();
		}

		auto const new_node_type = is_odd(contained_node_id) ? EVEN : ODD;
		set_node_type(new_node_id, new_node_type);

		edges.emplace_back(contained_node_id, new_node_id);
	}

	bool is_even(NodeId const node_id) const
	{
		return node_types.at(node_id) == EVEN;
	}

	bool is_odd(NodeId const node_id) const
	{
		return node_types.at(node_id) == ODD;
	}

	bool contains(NodeId const node_id) const
	{
		return node_types.at(node_id) != NOT_CONTAINED;
	}

	void set_node_type(NodeId const node_id, NodeType const node_type)
	{
		node_types.at(node_id) = node_type;
	}

private:

	ED::Graph const &G;

	/**
	 * Contains the edges in the alternating tree.
	 * All edges are oriented away from the root.
	 */
	Edge::Vector edges;

	/**
	 * Contains exactly one entry for every node such that
	 * node_types.at(node_id) is the value for the Node with id node_id.
	 */
	std::vector<NodeType> node_types;
};


#endif //COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H
