#ifndef COMBINATORIAL_OPTIMIZATION_CARDINALITYMATCHINGALGORITHM_H
#define COMBINATORIAL_OPTIMIZATION_CARDINALITYMATCHINGALGORITHM_H

#include <iostream>
#include "Matching.h"
#include "AlternatingTree.h"
#include "MatchingUnshrinkAlgorithm.h"

namespace algorithms {

class CardinalityMatchingAlgorithm
{
public:

	/**
	 * Compute a maximum matching.
	 */
	Matching run(ShrinkableGraph &G) const;

	/**
	 * Improve a given matching to maximum cardinality.
	 * @param G graph
	 * @param M hint matching
	 */
	Matching run(ShrinkableGraph &G, Matching &M) const;

private:
	void unshrink_matching(Matching &M, ShrinkableGraph const &G) const;

	void shrink_circuit(
			OddCircuit const &C,
			ShrinkableGraph &G,
			AlternatingTree &T,
			Matching &M,
			Edge::Vector &L
	) const;

	OddCircuit create_circuit(Edge const &edge, AlternatingTree const &T, ShrinkableGraph const &G) const;

	Edge pop(Edge::Vector &L) const;

	std::vector<NodeId> get_even_edge_vertices(Edge const &edge, AlternatingTree const &T) const;

	std::unique_ptr<AlternatingTree> reset_alternating_tree(
			Matching const &M,
			ShrinkableGraph const &G,
			Edge::Vector &L
	) const;

	void greedy_matching(Matching &M, ShrinkableGraph const &G) const;

};

}

#endif //COMBINATORIAL_OPTIMIZATION_CARDINALITYMATCHINGALGORITHM_H
