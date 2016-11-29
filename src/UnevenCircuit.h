

#ifndef COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
#define COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H


#include "Edge.h"
#include "utils.h"
#include <algorithm>
#include <memory>

class ShrinkableGraph;

class UnevenCircuit
{
public:
	using SharedPtr = std::shared_ptr<UnevenCircuit>;

	UnevenCircuit(
			ShrinkableGraph const &graph,
			std::vector<Edge> const &edges
	);

	Edge::Vector get_matching_edges_that_expose_one(NodeId const exposed_node_id) const;

	Edge::Vector const &get_edges() const;

	std::vector<NodeId> get_node_ids() const;

	bool operator==(UnevenCircuit const &rhs) const;

private:
	bool edges_are_ordered_correctly() const;

	template<typename Iterator>
	Edge::Vector take_every_second(Iterator iterator, Iterator const &end) const;

	ShrinkableGraph const &graph;
	Edge::Vector edges;
};


#endif //COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
