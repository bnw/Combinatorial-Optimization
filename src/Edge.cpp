

#include "Edge.h"

Edge::Edge(NodeId const first_node_id, NodeId const second_node_id) :
		node_ids({first_node_id, second_node_id})
{}

Edge Edge::reverse() const
{
	return {second_node_id(), first_node_id()};
}

Edge Edge::invalid_edge()
{
	return {ED::invalid_node_id, ED::invalid_node_id};
}

bool Edge::is_invalid_edge() const
{
	return first_node_id() == ED::invalid_node_id or second_node_id() == ED::invalid_node_id;
}

std::vector<NodeId> Edge::get_node_ids() const
{
	return {first_node_id(), second_node_id()};
}

NodeId Edge::first_node_id() const
{
	return node_ids.first;
}

NodeId Edge::second_node_id() const
{
	return node_ids.second;
}

bool Edge::operator==(Edge const &rhs) const
{
	return node_ids == rhs.node_ids or
		   (node_ids.first == rhs.node_ids.second and node_ids.second == rhs.node_ids.first);
}

bool Edge::operator!=(Edge const &rhs) const
{
	return not(rhs == *this);
}

bool Edge::operator<(Edge const &rhs) const
{
	if (std::min(node_ids.first, node_ids.second) < std::min(rhs.node_ids.first, rhs.node_ids.second)) {
		return true;
	} else if (std::min(node_ids.first, node_ids.second) > std::min(rhs.node_ids.first, rhs.node_ids.second)) {
		return false;
	}
	return std::max(node_ids.first, node_ids.second) < std::max(rhs.node_ids.first, rhs.node_ids.second);
}

NodeId Edge::other_vertex(NodeId const node_id) const
{
	assert(first_node_id() == node_id or second_node_id() == node_id);
	if (first_node_id() == node_id) {
		return second_node_id();
	} else {
		return first_node_id();
	}
}
