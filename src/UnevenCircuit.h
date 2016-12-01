

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

	Edge::Vector const get_even_path(NodeId const node_id_a, NodeId const node_id_b) const
	{
		enum part_names
		{
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};
		std::vector<Edge::Vector> parts(3);
		auto current_part_name = LEFT;
		for (auto const &edge : edges) {
			if (edge.first_node_id() == node_id_a or edge.first_node_id() == node_id_b) {
				current_part_name = (part_names) (current_part_name + 1);
			}
			parts.at(current_part_name).push_back(edge);
		}
		assert(current_part_name == RIGHT);
		if (parts.at(MIDDLE).size() % 2 == 0) {
			return parts.at(MIDDLE);
		} else {
			append(parts.at(LEFT), parts.at(RIGHT));
			assert(parts.at(LEFT).size() % 2 == 0);
			return parts.at(LEFT);
		}
	}

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
