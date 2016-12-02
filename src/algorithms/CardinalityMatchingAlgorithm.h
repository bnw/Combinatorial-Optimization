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
	Matching run(ShrinkableGraph &G) const
	{
		Matching M(G);
		while (M.has_exposed_vertex()) {
			NodeId const r = M.get_one_exposed();
			std::unique_ptr<AlternatingTree> T(new AlternatingTree(G, r));
			auto L = G.get_incident_edges(r);
			while (not L.empty()) {
				auto const &T_debug = *T;
				auto const edge = L.back();
				L.pop_back();
				if (G.is_contracted_edge(edge) or T->contains_edge(edge)) {
					continue;
				}
				auto even_vertices = filter<NodeId>(
						edge.get_node_ids(),
						[&](NodeId n) { return T->is_even(n); }
				);
				if (even_vertices.size() == 2) {
					auto circuit_edges = T->find_path(even_vertices.at(0), even_vertices.at(1));
					circuit_edges.push_back({even_vertices.at(1), even_vertices.at(0)});
					//TODO
					OddCircuit C(G, circuit_edges);
					for (auto const node_id : C.get_node_ids()) {
						assert(T->contains_node(node_id));
						if (T->is_odd(node_id)) {
							append(L, G.get_incident_edges(node_id));
						}
					}
					G.shrink_circuit(C);
					T->shrink_circuit(C);
					M.remove_edges_if_contained(circuit_edges);
				} else if (even_vertices.size() == 1) {
					auto const even_vertex = even_vertices.at(0);
					auto const other_vertex = edge.other_vertex(even_vertex);
					if (T->is_odd(other_vertex)) {
						continue;
					}
					if (M.is_exposed(other_vertex)) {
						auto augmenting_path = T->find_path_from_root_to_node(even_vertex);
						augmenting_path.push_back(edge);
						M.augment(augmenting_path);
						MatchingUnshrinkAlgorithm(M, G);
						if (M.is_perfect()) {
							std::cout << "Found perfect matching with " << M.get_num_edges() << " edges!"
									  << std::endl;
							return M;
						}
						G.reset();
						auto const new_root = M.get_one_exposed();
						T = std::unique_ptr<AlternatingTree>{new AlternatingTree(G, new_root)};
						L = G.get_incident_edges(new_root);
					} else {
						assert(M.is_covered(other_vertex));
						assert(not T->contains_node(other_vertex));
						T->add_edge(edge);
						auto const covering_edge = M.get_covering_edge(other_vertex);
						auto const z = covering_edge.other_vertex(other_vertex);
						T->add_edge(covering_edge);
						append(L, G.get_incident_edges(z));
					}
				}
			}
			//TODO remove debug
			MatchingUnshrinkAlgorithm(M, G);
			G.reset();
			G.deactivate_nodes(T->get_nodes());

//			std::cout << "|M| = " << M.get_num_edges()
//					  << ", G.num_active_nodes = " << G.get_num_active_nodes() << std::endl;
		}
		return M;
	}
};

}

#endif //COMBINATORIAL_OPTIMIZATION_CARDINALITYMATCHINGALGORITHM_H
