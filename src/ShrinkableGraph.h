#ifndef COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H
#define COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H

#include <graph.hpp>
#include "Edge.h"
#include "utils.h"
#include "UnevenCircuit.h"
#include <vector>
#include <memory>

class ShrinkableGraph : public ED::Graph
{
public:
	using PartitionClassId = size_t;

	ShrinkableGraph(size_type const num_nodes) :
			Graph(num_nodes),
			partition_classes(create_0_to_n_minus_one<PartitionClassId>(num_nodes)),
			pseudo_nodes(num_nodes, false),
			node_ids(create_0_to_n_minus_one<NodeId>(num_nodes))
	{}

	void shrink_circuit(UnevenCircuit const &uneven_circuit)
	{
		shrunken_circuits.push_front(uneven_circuit);
		for (auto const &edge : uneven_circuit.get_edges()) {
			shrink_edge(edge.first_node_id(), edge.second_node_id());
		}
	}

	//public for unit tests
	void shrink_edge(NodeId const node_a_id, NodeId const node_b_id)
	{
		//Note: This could be done faster
		auto const node_a_partition_class = get_partition_class_id(node_a_id);
		auto const node_b_partition_class = get_partition_class_id(node_b_id);
		for (auto const node_id : node_ids) {
			if (get_partition_class_id(node_id) == node_a_partition_class) {
				set_partition_class_id(node_id, node_b_partition_class);
			}
		}
		mark_as_pseudo_node(node_a_id);
		mark_as_pseudo_node(node_b_id);
	}

	Edge::Vector get_incident_edges(NodeId const node_id) const
	{
		Edge::Vector edges;
		auto const &node = this->node(node_id);
		for (auto const neighbor_id : node.neighbors()) {
			edges.emplace_back(node_id, neighbor_id);
		}
		return edges;
	}

	bool is_contracted_edge(Edge const &edge) const
	{
		return is_pseudo_node(edge.first_node_id())
			   and is_pseudo_node(edge.second_node_id());
	}

	std::list<UnevenCircuit> const &get_shrunken_circuits() const
	{
		return shrunken_circuits;
	}

private:
	PartitionClassId get_partition_class_id(NodeId const node_id) const
	{
		return partition_classes.at(node_id);
	}

	void set_partition_class_id(NodeId const node_id, PartitionClassId const partition_class_id)
	{
		partition_classes.at(node_id) = partition_class_id;
	}

	bool is_pseudo_node(NodeId const node_id) const
	{
		return pseudo_nodes.at(node_id);
	}

	void mark_as_pseudo_node(NodeId const node_id)
	{
		pseudo_nodes.at(node_id) = true;
	}

	std::vector<PartitionClassId> partition_classes;
	std::vector<bool> pseudo_nodes;
	std::vector<NodeId> const node_ids;
	std::list<UnevenCircuit> shrunken_circuits;
};

#endif //COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H
