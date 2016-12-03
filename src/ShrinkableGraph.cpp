

#include "ShrinkableGraph.h"

ShrinkableGraph::ShrinkableGraph(ED::Graph::size_type const num_nodes) :
		Graph(num_nodes),
		partition(num_nodes),
		active_nodes(num_nodes, true),
		shrunken_circuits_by_node(num_nodes)
{
	reset();
}

bool ShrinkableGraph::is_active(NodeId const node_id) const
{
	return active_nodes.at(node_id);
}

bool ShrinkableGraph::is_active(Edge const &edge) const
{
	return is_active(edge.first_node_id()) and is_active(edge.second_node_id());
}

void ShrinkableGraph::deactivate_nodes(std::vector<NodeId> const &node_ids)
{
	for (auto const node_id : node_ids) {
		assert(is_active(node_id));
		active_nodes.at(node_id) = false;
		assert(not is_active(node_id));
	}
}

void ShrinkableGraph::shrink_circuit(OddCircuit const &uneven_circuit)
{
	auto const shared_odd_circuit = std::make_shared<OddCircuit>(uneven_circuit);
	shared_odd_circuit->set_id(shrunken_circuits.size());
	shrunken_circuits.push_back(shared_odd_circuit);

	for (auto const &edge : uneven_circuit.get_edges()) {
		assert(is_active(edge));
		if (not is_contracted_edge(edge)) {
			shrink_edge(edge.first_node_id(), edge.second_node_id());
		}
		for (auto const node_id : edge.get_node_ids()) {
			shrunken_circuits_by_node.at(node_id).push_back(shared_odd_circuit);
		}
	}
}

void ShrinkableGraph::shrink_edge(NodeId const node_a_id, NodeId const node_b_id)
{
	partition.join(node_a_id, node_b_id);

	mark_as_pseudo_node(node_a_id);
	mark_as_pseudo_node(node_b_id);
}

Edge::Vector ShrinkableGraph::get_incident_edges(NodeId const node_id) const
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

bool ShrinkableGraph::is_contracted_edge(Edge const &edge) const
{
	assert(is_active(edge));
	return are_equal(edge.first_node_id(), edge.second_node_id());
}

std::vector<OddCircuit::SharedPtr> const &ShrinkableGraph::get_shrunken_circuits() const
{
	return shrunken_circuits;
}

bool ShrinkableGraph::is_pseudo_node(NodeId const node_id) const
{
	assert(is_active(node_id));
	return pseudo_nodes.at(node_id);
}

bool ShrinkableGraph::are_equal(NodeId const node_id_a, NodeId const node_id_b) const
{
	return get_partition_class_id(node_id_a) == get_partition_class_id(node_id_b);
}

void ShrinkableGraph::reset()
{
	partition.reset();
	pseudo_nodes = std::vector<bool>(num_nodes(), false);
	shrunken_circuits.clear();
	shrunken_circuits_by_node = std::vector<std::vector<OddCircuit::SharedPtr>>(num_nodes());
}

std::vector<OddCircuit::SharedPtr> const &ShrinkableGraph::get_shrunken_circuits(NodeId const node_id) const
{
	return shrunken_circuits_by_node.at(node_id);
}

size_t ShrinkableGraph::get_num_active_nodes() const
{
	size_t num_active_nodes = 0;
	for (auto const is_active : active_nodes) {
		num_active_nodes += is_active;
	}
	return num_active_nodes;
}

void ShrinkableGraph::mark_as_pseudo_node(NodeId const node_id)
{
	pseudo_nodes.at(node_id) = true;
}

NodeId ShrinkableGraph::get_partition_class_id(NodeId const node_id) const
{
	return partition.find(node_id);
}
