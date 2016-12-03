#include "MatchingUnshrinkAlgorithm.h"


namespace algorithms {

MatchingUnshrinkAlgorithm::MatchingUnshrinkAlgorithm(
		Matching &M,
		ShrinkableGraph const &graph
)
		: M(M),
		  graph(graph),
		  num_circuits(graph.get_shrunken_circuits().size()),
		  nodes_to_expand(M.get_covered_nodes()),
		  circuits_at_node_have_already_been_expanded(graph.num_nodes(), false),
		  circuit_already_expanded(num_circuits, false),
		  index_of_outermost_circuit_that_might_not_be_expanded((int) num_circuits - 1)
{
	if (nodes_to_expand.empty()) {
		expand_outermost_circuit();
	}

	while (not nodes_to_expand.empty()) {
		auto const node_id = nodes_to_expand.back();
		nodes_to_expand.pop_back();

		expand_circuits_at_node(node_id);

		if (nodes_to_expand.empty()) {
			expand_outermost_circuit();
		}
	}

	for (auto has_been_expanded : circuit_already_expanded) {
		assert(has_been_expanded);
	}
}

void MatchingUnshrinkAlgorithm::expand_outermost_circuit()
{
	for (int &circuit_id = index_of_outermost_circuit_that_might_not_be_expanded; circuit_id >= 0; circuit_id--) {
		if (circuit_already_expanded.at((size_t) circuit_id)) {
			continue;
		}
		auto const &circuit = *graph.get_shrunken_circuits().at((size_t) circuit_id);
		expand_circuit(circuit, circuit.get_node_ids().front());
		break;
	}
}

void MatchingUnshrinkAlgorithm::expand_circuit(OddCircuit const &circuit, NodeId const exposed_node)
{
	circuit_already_expanded.at(circuit.get_id()) = true;
	auto const new_matching_edges = circuit.get_matching_edges_that_expose_one(exposed_node);
	add_matching_edges(new_matching_edges);
}

void MatchingUnshrinkAlgorithm::add_matching_edges(Edge::Vector const &new_edges)
{
	M.add_edges(new_edges);
	for (auto const &edge : new_edges) {
		append(nodes_to_expand, edge.get_node_ids());
	}
}

void MatchingUnshrinkAlgorithm::expand_circuits_at_node(NodeId const node_id)
{
	if (circuits_at_node_have_already_been_expanded.at(node_id)) {
		return;
	}
	for (auto const &circuit : graph.get_shrunken_circuits(node_id)) {
		if (circuit_already_expanded.at(circuit->get_id())) {
			continue;
		}
		expand_circuit(*circuit, node_id);
	}
	circuits_at_node_have_already_been_expanded.at(node_id) = true;
}

}