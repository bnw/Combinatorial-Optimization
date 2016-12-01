#include <set>
#include "UnevenCircuit.h"
#include "ShrinkableGraph.h"

UnevenCircuit::UnevenCircuit(ShrinkableGraph const &graph, std::vector<Edge> const &edges)
		: graph(graph),
		  edges(edges),
		  id(0)
{
	for (auto const &edge : edges) {
		edge_is_contracted.push_back(graph.is_contracted_edge(edge));
	}
	assert(edges_form_a_circuit());
	assert(edges.size() % 2 == 1);
}

Edge::Vector UnevenCircuit::get_matching_edges_that_expose_one(NodeId const exposed_node_id) const
{
	std::vector<size_t> const edge_positions = create_0_to_n_minus_one(edges.size());
	std::vector<size_t> position_matching_of_edges;

	for (auto edge_position_it = edge_positions.begin(); edge_position_it != edge_positions.end(); edge_position_it++) {
		auto const &edge = edges.at(*edge_position_it);
		if (edge.first_node_id() == exposed_node_id) {
			append(position_matching_of_edges,
				   take_every_second(
						   edge_position_it,
						   edge_positions.end(),
						   [&](size_t i) { return not edge_is_contracted.at(i); }
				   )
			);
			append(position_matching_of_edges,
				   take_every_second(
						   std::reverse_iterator<decltype(edge_position_it)>(edge_position_it),
						   edge_positions.rend(),
						   [&](size_t i) { return not edge_is_contracted.at(i); }
				   )
			);
			break;
		}
		if (edge.second_node_id() == exposed_node_id) {
			append(position_matching_of_edges,
				   take_every_second(
						   edge_position_it + 1,
						   edge_positions.end(),
						   [&](size_t i) { return not edge_is_contracted.at(i); }
				   )
			);
			append(position_matching_of_edges,
				   take_every_second(
						   std::reverse_iterator<decltype(edge_position_it)>(edge_position_it + 1),
						   edge_positions.rend(),
						   [&](size_t i) { return not edge_is_contracted.at(i); }
				   )
			);
			break;
		}
	}

	assert(position_matching_of_edges.size() == edges.size() / 2);

	return transform<Edge>(position_matching_of_edges, [&](size_t i) { return edges.at(i); });
}

Edge::Vector const &UnevenCircuit::get_edges() const
{
	return edges;
}

std::vector<NodeId> UnevenCircuit::get_node_ids() const
{
	return transform<NodeId>(get_edges(), [](Edge const &edge) { return edge.first_node_id(); });
}

bool UnevenCircuit::operator==(UnevenCircuit const &rhs) const
{
	return edges == rhs.edges;
}

bool UnevenCircuit::edges_form_a_circuit() const
{
	for (auto edge_it = edges.begin(); edge_it != edges.end() - 1; edge_it++) {
		if (not graph.are_equal(edge_it->second_node_id(), (edge_it + 1)->first_node_id())) {
			return false;
		}
		assert(not graph.is_contracted_edge(*edge_it));
	}
	return graph.are_equal(edges.back().second_node_id(), edges.front().first_node_id());
}

template<typename Iterator>
std::vector<typename Iterator::value_type> UnevenCircuit::take_every_second(
		Iterator iterator,
		Iterator const &end,
		std::function<bool(typename Iterator::value_type const &)> const &filter
) const
{
	std::vector<typename Iterator::value_type> result;
	bool is_even = false;
	for (; iterator != end; iterator++) {
		if (filter(*iterator)) {
			if (is_even) {
				result.push_back(*iterator);
			}
			is_even = not is_even;
		}
	}
	return result;
}

void UnevenCircuit::set_id(size_t id)
{
	UnevenCircuit::id = id;
}

size_t UnevenCircuit::get_id() const
{
	return id;
}
