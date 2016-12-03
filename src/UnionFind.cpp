#include "UnionFind.h"

UnionFind::UnionFind(size_t const size)
{
	for (size_t i = 0; i < size; i++) {
		nodes.push_back(std::make_shared<UnionFindNode>(i, nullptr, 0));
	}
}

NodeId UnionFind::find(NodeId const node_id) const
{
	return get_node(node_id)->get_root()->node_id;
}

void UnionFind::join(NodeId const node_id_a, NodeId const node_id_b)
{
	auto root_a = get_node(node_id_a)->get_root();
	auto root_b = get_node(node_id_b)->get_root();
	if (root_a->rank < root_b->rank) {
		root_a->parent = root_b;
	} else if (root_a->rank > root_b->rank) {
		root_b->parent = root_a;
	} else {
		root_a->parent = root_b;
		root_b->rank++;
	}
}

UnionFind::UnionFindNode::SharedPtr const &UnionFind::get_node(NodeId const node_id) const
{
	return nodes.at(node_id);
}

UnionFind::UnionFindNode::UnionFindNode(NodeId node_id, UnionFind::UnionFindNode::SharedPtr const &parent, size_t rank)
		: node_id(node_id),
		  parent(parent),
		  rank(rank)
{}

UnionFind::UnionFindNode::SharedPtr UnionFind::UnionFindNode::get_root()
{
	SharedPtr result;
	if (parent) {
		result = parent->get_root();
		parent = result;
	} else {
		result = shared_from_this();
	}
	return result;
}