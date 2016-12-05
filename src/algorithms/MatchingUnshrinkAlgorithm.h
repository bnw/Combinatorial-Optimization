#ifndef COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H
#define COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H

#include <Matching.h>

namespace algorithms {

class MatchingUnshrinkAlgorithm
{
public:
	/**
	 * Adds matching edges to M for all odd circuits.
	 * The circuits must be considered in the following order:
	 * 1) If there are unexpanded circuits incident to a matching edge, expand these first.
	 * 2) If all unexpanded circuits are not incident to any matching edges, expand one of
	 *    them and continue with step 1).
	 * Complexity: O(m + num_circuits) = O(m)
	 * Note that there are at most 2*m entries in graph.circuits_by_node.
	 */
	MatchingUnshrinkAlgorithm(
			Matching &M,
			ShrinkableGraph const &graph
	);

private:
	void expand_circuits_at_node(NodeId const node_id);

	/**
	 * Note: Amortized runtime over all calls to this function will be O(m).
	 */
	void expand_outermost_circuit();

	/**
	 * Complexity: O(|circuit.edges|)
	 */
	void expand_circuit(OddCircuit const &circuit, NodeId const exposed_node);

	/**
	 * Adds new_edges to M and also queues all nodes of the edges for expansion,
	 * by adding them to nodes_to_expand.
	 */
	void add_matching_edges(Edge::Vector const &new_edges);

	Matching &M;
	ShrinkableGraph const &graph;
	size_t const num_circuits;
	std::vector<NodeId> nodes_to_expand;
	std::vector<bool> circuits_at_node_have_already_been_expanded;
	std::vector<bool> circuit_already_expanded;
	int index_of_outermost_circuit_that_might_not_be_expanded;
};

}

#endif //COMBINATORIAL_OPTIMIZATION_MATCHINGUNSHRINKALGORITHM_H
