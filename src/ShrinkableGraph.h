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
			node_ids(create_0_to_n_minus_one<PartitionClassId>(num_nodes)),
			active_nodes(num_nodes, true)
	{
		reset();
	}

	bool is_active(NodeId const node_id) const
	{
		return active_nodes.at(node_id);
	}

	bool is_active(Edge const &edge) const
	{
		return is_active(edge.first_node_id()) and is_active(edge.second_node_id());
	}

	void deactivate_nodes(std::vector<NodeId> const &node_ids)
	{
		for (auto const node_id : node_ids) {
			assert(is_active(node_id));
			active_nodes.at(node_id) = false;
			assert(not is_active(node_id));
		}
	}

	void shrink_circuit(UnevenCircuit const &uneven_circuit)
	{
		shrunken_circuits.push_front(uneven_circuit);

		//TODO remove debug
		int num_contracted_edges = 0;
		for (auto const &edge : uneven_circuit.get_edges()) {
			assert(is_active(edge));
			num_contracted_edges += is_contracted_edge(edge);
			if(not is_contracted_edge(edge)) {
				shrink_edge(edge.first_node_id(), edge.second_node_id());
			}
		}
		assert(num_contracted_edges == 1);
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
		assert(is_active(node_id));
		Edge::Vector edges;
		for (auto const neighbor_id : node(node_id).neighbors()) {
			if (is_active(neighbor_id)) {
				edges.emplace_back(node_id, neighbor_id);
			}
		}
		return edges;
	}

	bool is_contracted_edge(Edge const &edge) const
	{
		assert(is_active(edge));
		return are_equal(edge.first_node_id(), edge.second_node_id());
//		return is_pseudo_node(edge.first_node_id())
//			   and is_pseudo_node(edge.second_node_id());
	}

	std::list<UnevenCircuit> const &get_shrunken_circuits() const
	{
		return shrunken_circuits;
	}

	bool is_pseudo_node(NodeId const node_id) const
	{
		assert(is_active(node_id));
		return pseudo_nodes.at(node_id);
	}

	bool are_equal(NodeId const node_id_a, NodeId const node_id_b) const
	{
		return get_partition_class_id(node_id_a) == get_partition_class_id(node_id_b);
	}

	bool are_equal(Edge const &edge_a, Edge const &edge_b) const
	{
		return (get_partition_class_id(edge_a.first_node_id()) == get_partition_class_id(edge_b.first_node_id())
				and get_partition_class_id(edge_a.second_node_id()) == get_partition_class_id(edge_b.second_node_id()))
			   or
			   (get_partition_class_id(edge_a.first_node_id()) == get_partition_class_id(edge_b.second_node_id())
				and get_partition_class_id(edge_a.second_node_id()) == get_partition_class_id(edge_b.first_node_id()));
	}

	PartitionClassId get_partition_class_id(NodeId const node_id) const
	{
		return partition_classes.at(node_id);
	}

	void reset()
	{
		partition_classes = create_0_to_n_minus_one<PartitionClassId>(num_nodes());
		pseudo_nodes = std::vector<bool>(num_nodes(), false);
		shrunken_circuits.clear();
	}

private:

	void set_partition_class_id(NodeId const node_id, PartitionClassId const partition_class_id)
	{
		partition_classes.at(node_id) = partition_class_id;
	}

	void mark_as_pseudo_node(NodeId const node_id)
	{
		pseudo_nodes.at(node_id) = true;
	}

	std::vector<PartitionClassId> partition_classes;
	std::vector<bool> pseudo_nodes;
	std::list<UnevenCircuit> shrunken_circuits;
	std::vector<NodeId> const node_ids;
	std::vector<bool> active_nodes;
};

#endif //COMBINATORIAL_OPTIMIZATION_SHRINKABLEGRAPH_H
