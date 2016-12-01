

#ifndef COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
#define COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H


#include "Edge.h"
#include "utils.h"
#include <algorithm>
#include <memory>
#include <map>

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

	void set_id(size_t id);

	size_t get_id() const;

private:
	bool edges_form_a_circuit() const;

	template<typename Iterator>
	std::vector<typename Iterator::value_type> take_every_second(
			Iterator iterator,
			Iterator const &end,
			std::function<bool(typename Iterator::value_type const &)> const &filter
	) const;

	ShrinkableGraph const &graph;
	Edge::Vector edges;
	std::vector<bool> edge_is_contracted;
	size_t id;
};


#endif //COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
