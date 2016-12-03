#include <set>
#include "OddCircuit.h"
#include "ShrinkableGraph.h"

OddCircuit::OddCircuit(ShrinkableGraph const &graph, std::vector<Edge> const &edges)
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

Edge::Vector OddCircuit::get_matching_edges_that_expose_one(NodeId const exposed_node_id) const
{
	std::vector<size_t> const edge_positions = create_0_to_n_minus_one(edges.size());
	std::vector<size_t> position_matching_of_edges;

	for (auto edge_position_it = edge_positions.begin(); edge_position_it != edge_positions.end(); edge_position_it++) {
		auto const &edge = edges.at(*edge_position_it);
		auto const is_contracted_edge_fn = [&](size_t i) { return not edge_is_contracted.at(i); };
		if (edge.first_node_id() == exposed_node_id) {
			append(position_matching_of_edges,
				   take_every_second(
						   edge_position_it,
						   edge_positions.end(),
						   is_contracted_edge_fn
				   )
			);
			append(position_matching_of_edges,
				   take_every_second(
						   std::reverse_iterator<decltype(edge_position_it)>(edge_position_it),
						   edge_positions.rend(),
						   is_contracted_edge_fn
				   )
			);
			break;
		}
		if (edge.second_node_id() == exposed_node_id) {
			append(position_matching_of_edges,
				   take_every_second(
						   edge_position_it + 1,
						   edge_positions.end(),
						   is_contracted_edge_fn
				   )
			);
			append(position_matching_of_edges,
				   take_every_second(
						   std::reverse_iterator<decltype(edge_position_it)>(edge_position_it + 1),
						   edge_positions.rend(),
						   is_contracted_edge_fn
				   )
			);
			break;
		}
	}

	assert(position_matching_of_edges.size() == edges.size() / 2);

	return transform<Edge>(position_matching_of_edges, [&](size_t i) { return edges.at(i); });
}

Edge::Vector const &OddCircuit::get_edges() const
{
	return edges;
}

std::vector<NodeId> OddCircuit::get_node_ids() const
{
	return transform<NodeId>(get_edges(), [](Edge const &edge) { return edge.first_node_id(); });
}

bool OddCircuit::operator==(OddCircuit const &rhs) const
{
	return edges == rhs.edges;
}

bool OddCircuit::edges_form_a_circuit() const
{
	for (auto edge_it = edges.begin(); edge_it != edges.end() - 1; edge_it++) {
		if (not graph.are_equal(edge_it->second_node_id(), (edge_it + 1)->first_node_id())) {
			return false;
		}
		assert(not graph.is_contracted_edge(*edge_it));
	}
	return graph.are_equal(edges.back().second_node_id(), edges.front().first_node_id());
}

template<typename Iterator, typename Filter>
std::vector<typename Iterator::value_type> OddCircuit::take_every_second(
		Iterator iterator,
		Iterator const &end,
		Filter &filter
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

void OddCircuit::set_id(size_t id)
{
	OddCircuit::id = id;
}

size_t OddCircuit::get_id() const
{
	return id;
}
