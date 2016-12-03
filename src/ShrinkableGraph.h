#ifndef COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H
#define COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H

#include <graph.hpp>
#include "Edge.h"
#include "utils.h"
#include "OddCircuit.h"
#include "UnionFind.h"
#include <vector>
#include <memory>

class ShrinkableGraph : public ED::Graph
{
public:
	using PartitionClassId = size_t;

	ShrinkableGraph(size_type const num_nodes);

	bool is_active(NodeId const node_id) const;

	bool is_active(Edge const &edge) const;

	/**
	 * Deactivates a node.
	 * Edges to deactivated nodes will not be considered in get_incident_edges.
	 */
	void deactivate_nodes(std::vector<NodeId> const &node_ids);

	/**
	 * Contracts all edges in the circuit.
	 * Complexity: Linear in the number of edges of the given circuit.
	 */
	void shrink_circuit(OddCircuit const &uneven_circuit);

	//public for unit tests
	void shrink_edge(NodeId const node_a_id, NodeId const node_b_id);

	/**
	 * Returns all edges incident to node_id.
	 * Does not return edges that are incident to deactivated nodes.
	 * Complexity: Linear in the number of neighbors of node_id.
	 */
	Edge::Vector get_incident_edges(NodeId const node_id) const;

	bool is_contracted_edge(Edge const &edge) const;

	bool is_pseudo_node(NodeId const node_id) const;

	/**
	 * Returns true iff both nodes are part of the same pseudo node.
	 */
	bool are_equal(NodeId const node_id_a, NodeId const node_id_b) const;

	/**
	 * Undos all shrinkings.
	 * Does NOT reset the deactivated nodes.
	 */
	void reset();

	/**
	 * Returns all the circuits used to shrink the graph since the last reset().
	 */
	std::vector<OddCircuit::SharedPtr> const &get_shrunken_circuits() const;

	/**
	 * Returns all the circuits that have one edge incident to node_id.
	 */
	std::vector<OddCircuit::SharedPtr> const &get_shrunken_circuits(NodeId const node_id) const;

	/**
	 * For debugging
	 */
	size_t get_num_active_nodes() const;

private:
	void mark_as_pseudo_node(NodeId const node_id);

	NodeId get_partition_class_id(NodeId const node_id) const;

	/** A partition of all nodes. */
	UnionFind partition;

	/** pseudo_nodes.at(node_id) indicates if node_id belongs to any shrunken circuit. */
	std::vector<bool> pseudo_nodes;

	std::vector<bool> active_nodes;

	std::vector<OddCircuit::SharedPtr> shrunken_circuits;

	std::vector<std::vector<OddCircuit::SharedPtr>> shrunken_circuits_by_node;
};

#endif //COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H
