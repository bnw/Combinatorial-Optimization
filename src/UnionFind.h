#ifndef COMBINATORIAL_OPTIMIZATION_UNIONFIND_H
#define COMBINATORIAL_OPTIMIZATION_UNIONFIND_H


#include <memory>
#include <vector>
#include "typedefs.h"

class UnionFind
{
private:
	struct UnionFindNode : public std::enable_shared_from_this<UnionFindNode>
	{
		using SharedPtr = std::shared_ptr<UnionFindNode>;
		NodeId node_id;
		SharedPtr parent;
		size_t rank;

		UnionFindNode(NodeId node_id, SharedPtr const &parent, size_t rank);

		SharedPtr get_root();
	};

public:
	UnionFind(size_t const size);

	NodeId find(NodeId const node_id) const;

	void join(NodeId const node_id_a, NodeId const node_id_b);

	void reset()
	{
		for (auto const &node : nodes) {
			node->parent = nullptr;
			node->rank = 0;
		}
	}

private:
	UnionFindNode::SharedPtr const &get_node(NodeId const node_id) const;

	std::vector<UnionFindNode::SharedPtr> nodes;

};


#endif //COMBINATORIAL_OPTIMIZATION_UNIONFIND_H
