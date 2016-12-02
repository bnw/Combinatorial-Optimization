#include "AlternatingTree.h"

void AlternatingTree::add_edge(Edge const &edge)
{
	assert(contains_node(edge.first_node_id()) xor contains_node(edge.second_node_id()));

	//Find out which endpoint is already in the alternating tree and which is new
	NodeId contained_node_id = contains_node(edge.first_node_id()) ?
							   edge.first_node_id() :
							   edge.second_node_id();
	NodeId new_node_id = edge.other_vertex(contained_node_id);

	auto const new_node_type = is_odd(contained_node_id) ? EVEN : ODD;

	set_node_type(new_node_id, new_node_type);
	edges.emplace_back(contained_node_id, new_node_id);
	node_levels.at(new_node_id) = node_levels.at(contained_node_id) + 1;
	ingoing_edges.at(new_node_id) = edges.back();
}

void AlternatingTree::shrink_circuit(OddCircuit const &C)
{
	for (auto n : C.get_node_ids()) {
		node_types.at(n) = EVEN;
	}
}

Edge::Vector AlternatingTree::find_path_from_root_to_node(NodeId const node_id) const
{
	return find_path(root_node_id, node_id);
}

Edge::Vector AlternatingTree::find_path(NodeId const start_node_id, NodeId const end_node_id) const
{
	auto current_node_a = start_node_id;
	auto current_node_b = end_node_id;
	Edge::List path_to_root;
	Edge::List path_from_root;
	while (current_node_a != current_node_b) {
		if (get_level(current_node_a) > get_level(current_node_b)) {
			path_to_root.push_back(get_ingoing_edge(current_node_a).reverse());
			current_node_a = get_ingoing_edge(current_node_a).other_vertex(current_node_a);
		} else {
			path_from_root.push_front(get_ingoing_edge(current_node_b));
			current_node_b = get_ingoing_edge(current_node_b).other_vertex(current_node_b);
		}
	}
	Edge::List path;
	append(path, path_to_root);
	append(path, path_from_root);

	path.remove_if([&](Edge const &edge) { return G.is_contracted_edge(edge); });

	return Edge::Vector(path.begin(), path.end());
}

bool AlternatingTree::is_even(NodeId const node_id) const
{
	return node_types.at(node_id) == EVEN;
}

bool AlternatingTree::is_odd(NodeId const node_id) const
{
	return node_types.at(node_id) == ODD;
}

bool AlternatingTree::contains_node(NodeId const node_id) const
{
	return node_types.at(node_id) != NOT_CONTAINED;
}

bool AlternatingTree::contains_edge(Edge const &edge) const
{
	return ingoing_edges.at(edge.first_node_id()) == edge
		   or ingoing_edges.at(edge.second_node_id()) == edge;
}

void AlternatingTree::set_node_type(NodeId const node_id, AlternatingTree::NodeType const node_type)
{
	node_types.at(node_id) = node_type;
}

std::vector<NodeId> AlternatingTree::get_nodes() const
{
	std::vector<NodeId> nodes = {root_node_id};
	for (auto const &edge : edges) {
		assert(edge.second_node_id() != root_node_id);
		nodes.push_back(edge.second_node_id());
	}
	return nodes;
}

unsigned AlternatingTree::get_level(NodeId const node_id) const
{
	return node_levels.at(node_id);
}

Edge AlternatingTree::get_ingoing_edge(NodeId const node_id) const
{
	auto const ingoing_edge = ingoing_edges.at(node_id);
	assert(ingoing_edge != Edge::invalid_edge());
	return ingoing_edge;
}
