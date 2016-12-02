#ifndef COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H
#define COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H

#include <cassert>
#include "ShrinkableGraph.h"
#include "Matching.h"

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
	AlternatingTree(ShrinkableGraph const &graph, NodeId const root_node_id) :
			G(graph),
			node_types(G.num_nodes(), NOT_CONTAINED),
			node_levels(G.num_nodes(), 0),
			ingoing_edges(G.num_nodes(), Edge::invalid_edge()),
			root_node_id(root_node_id)
	{
		node_types.at(root_node_id) = EVEN;
	}

	void add_edge(Edge const &edge);

	void shrink_circuit(OddCircuit const &C);

	Edge::Vector find_path_from_root_to_node(NodeId const node_id) const;

	/**
	 * Finds a path P in T from start_node_id to end_node_id
	 * containing only non-contracted edges.
	 * Complexity: O(|P|)
	 */
	Edge::Vector find_path(NodeId const start_node_id, NodeId const end_node_id) const;

	bool is_even(NodeId const node_id) const;

	bool is_odd(NodeId const node_id) const;

	bool contains_node(NodeId const node_id) const;

	bool contains_edge(Edge const &edge) const;

	/**
	 * Returns all the nodes in the tree.
	 * Complexity: Linear in the number of nodes in the tree.
	 */
	std::vector<NodeId> get_nodes() const;

private:
	void set_node_type(NodeId const node_id, NodeType const node_type);

	unsigned get_level(NodeId const node_id) const;

	Edge get_ingoing_edge(NodeId const node_id) const;

	ShrinkableGraph const &G;

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

	/**
	 * Contains exactly one entry for ever node such that
	 * node_levels.at(node_id) is the distance from node_id to the root.
	 */
	std::vector<unsigned> node_levels;

	/** Contains exactly one entry for ever node. */
	Edge::Vector ingoing_edges;

	NodeId const root_node_id;
};


#endif //COMBINATORIAL_OPTIMIZATION_ALTERNATINGTREE_H
