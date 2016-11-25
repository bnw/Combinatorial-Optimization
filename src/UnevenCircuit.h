

#ifndef COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
#define COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H


#include "Edge.h"
#include "utils.h"
#include <algorithm>
#include <memory>

class UnevenCircuit
{
public:
	using SharedPtr = std::shared_ptr<UnevenCircuit>;

	UnevenCircuit(
			ED::Graph const &graph,
			std::vector<Edge> const &edges
	) : edges(edges)
	{
		assert(edges.size() % 2 == 1);
	}

	Edge::Vector get_matching_edges_that_expose_one(NodeId const exposed_node_id) const
	{
		Edge::Vector matching_edges;
		for (auto edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
			if (edge_it->first_node_id() == exposed_node_id) {
				append(matching_edges, take_every_second(edge_it, edges.end()));
				append(matching_edges,
					   take_every_second(std::reverse_iterator<decltype(edge_it)>(edge_it), edges.rend()));
			}
		}
		return matching_edges;
	}

	Edge::Vector const &get_edges() const
	{
		return edges;
	}

	std::vector<NodeId> get_node_ids() const
	{
		std::vector<NodeId> node_ids;
		for (auto const &edge : get_edges()) {
			node_ids.push_back(edge.first_node_id());
		}
		return node_ids;
	}

private:
	template<typename Iterator>
	Edge::Vector take_every_second(Iterator iterator, Iterator const &end) const
	{
		Edge::Vector result;
		bool is_even = false;
		for (; iterator != end; iterator++, is_even = not is_even) {
			if (is_even) {
				result.push_back(*iterator);
			}
		}
		return result;
	}

	Edge::Vector edges;
};


#endif //COMBINATORIAL_OPTIMIZATION_UNEVENCIRCLE_H
