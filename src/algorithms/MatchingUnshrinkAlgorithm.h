#ifndef COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H
#define COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H

#include <Matching.h>

namespace algorithms {

class MatchingUnshrinkAlgorithm
{
public:
	MatchingUnshrinkAlgorithm(
			Matching &M,
			ShrinkableGraph const &graph
	) : M(M),
		graph(graph),
		num_circuits(graph.get_shrunken_circuits().size()),
		circuit_already_expanded(num_circuits, false),
		nodes_to_expand(M.get_covered_nodes()),
		index_of_outermost_circuit_that_might_not_be_expanded(num_circuits - 1)
	{
		if (nodes_to_expand.empty()) {
			expand_outermost_circuit();
			assert(not nodes_to_expand.empty());
		}

		while (not nodes_to_expand.empty()) {
			auto const node_id = nodes_to_expand.back();
			nodes_to_expand.pop_back();
			for (auto const &circuit : graph.get_circuits(node_id)) {
				if (circuit_already_expanded.at(circuit->get_id())) {
					continue;
				}
				circuit_already_expanded.at(circuit->get_id()) = true;
				auto const new_matching_edges = circuit->get_matching_edges_that_expose_one(node_id);
				add_matching_edges(new_matching_edges);
			}

			if (nodes_to_expand.empty()) {
				expand_outermost_circuit();
			}
		}

		for (auto has_been_expanded : circuit_already_expanded) {
			assert(has_been_expanded);
		}
	}

private:
	void expand_outermost_circuit()
	{
		for (int circuit_id = (int) num_circuits - 1; circuit_id >= 0; circuit_id--) {
			if (circuit_already_expanded.at((size_t) circuit_id)) {
				continue;
			}
			auto const &circuit = *graph.get_shrunken_circuits().at((size_t) circuit_id);
			assert(circuit.get_id() == circuit_id);
			circuit_already_expanded.at(circuit.get_id()) = true;
			auto const some_circuit_node = circuit.get_node_ids().front();
			auto const new_matching_edges = circuit.get_matching_edges_that_expose_one(some_circuit_node);
			add_matching_edges(new_matching_edges);
			assert(not nodes_to_expand.empty());
			break;
		}
	};

	void add_matching_edges(Edge::Vector const &new_edges)
	{
		M.add_edges(new_edges);
		for (auto const &edge : new_edges) {
			append(nodes_to_expand, edge.get_node_ids());
		}
	}

	Matching &M;
	ShrinkableGraph const &graph;
	size_t num_circuits;
	std::vector<NodeId> nodes_to_expand;
	std::vector<bool> circuit_already_expanded;
	size_t index_of_outermost_circuit_that_might_not_be_expanded;
};

}

#endif //COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H
