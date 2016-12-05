#include "CardinalityMatchingAlgorithm.h"

namespace algorithms {

Matching algorithms::CardinalityMatchingAlgorithm::run(ShrinkableGraph &G) const
{
	Matching M(G);
	return run(G, M);
}

Matching algorithms::CardinalityMatchingAlgorithm::run(ShrinkableGraph &G, Matching &M) const
{
	greedy_matching(M, G);
	while (M.has_exposed_vertex()) {
		Edge::Vector L;
		auto T = reset_alternating_tree(M, G, L);
		while (not L.empty()) {
			auto const edge = pop(L);
			if (G.is_contracted_edge(edge) or T->contains_edge(edge)) {
				continue;
			}
			auto even_vertices = get_even_edge_vertices(edge, *T);
			if (even_vertices.size() == 2) {
				auto const C = create_circuit(edge, *T, G);
				shrink_circuit(C, G, *T, M, L);
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
					unshrink_matching(M, G);
					if (M.is_perfect()) {
						return M;
					}
					G.reset();
					T = reset_alternating_tree(M, G, L);
				} else {
					auto const covering_edge = M.get_covering_edge(other_vertex);
					auto const z = covering_edge.other_vertex(other_vertex);
					T->add_edge(edge);
					T->add_edge(covering_edge);
					append(L, G.get_incident_edges(z));
				}
			}
		}
		unshrink_matching(M, G);
		G.reset();
		G.deactivate_nodes(T->get_nodes());

//			std::cout << "|M| = " << M.get_num_edges()
//					  << ", G.num_active_nodes = " << G.get_num_active_nodes() << std::endl;
	}
	return M;
}

void algorithms::CardinalityMatchingAlgorithm::unshrink_matching(Matching &M, ShrinkableGraph const &G) const
{
	MatchingUnshrinkAlgorithm(M, G);
}

void
algorithms::CardinalityMatchingAlgorithm::shrink_circuit(OddCircuit const &C, ShrinkableGraph &G, AlternatingTree &T,
		Matching &M, Edge::Vector &L) const
{
	for (auto const node_id : C.get_node_ids()) {
		assert(T.contains_node(node_id));
		if (T.is_odd(node_id)) {
			append(L, G.get_incident_edges(node_id));
		}
	}
	G.shrink_circuit(C);
	T.shrink_circuit(C);
	M.remove_edges_if_contained(C.get_edges());
}

OddCircuit algorithms::CardinalityMatchingAlgorithm::create_circuit(Edge const &edge, AlternatingTree const &T,
		ShrinkableGraph const &G) const
{
	auto circuit_edges = T.find_path(edge.first_node_id(), edge.second_node_id());
	circuit_edges.push_back({edge.second_node_id(), edge.first_node_id()});
	return OddCircuit(G, circuit_edges);
}

Edge algorithms::CardinalityMatchingAlgorithm::pop(Edge::Vector &L) const
{
	auto const edge = L.back();
	L.pop_back();
	return edge;
}

std::vector<unsigned long, std::allocator<unsigned long>>
algorithms::CardinalityMatchingAlgorithm::get_even_edge_vertices(Edge const &edge, AlternatingTree const &T) const
{
	return filter<NodeId>(
			edge.get_node_ids(),
			[&](NodeId n) { return T.is_even(n); }
	);
}

std::unique_ptr <AlternatingTree, std::default_delete<AlternatingTree>>
algorithms::CardinalityMatchingAlgorithm::reset_alternating_tree(Matching const &M, ShrinkableGraph const &G,
		Edge::Vector &L) const
{
	NodeId const r = M.get_one_exposed();
	L = G.get_incident_edges(r);
	return std::unique_ptr<AlternatingTree>(new AlternatingTree(G, r));
}

void CardinalityMatchingAlgorithm::greedy_matching(Matching &M, ShrinkableGraph const &G) const
{
	for (size_t node_id = 0; node_id < G.num_nodes(); node_id++) {
		if (M.is_covered(node_id)) {
			continue;
		}
		for (auto const neighbor_node_id : G.node(node_id).neighbors()) {
			if (not M.is_covered(neighbor_node_id)) {
				M.add_edge({node_id, neighbor_node_id});
				break;
			}
		}
	}
}

}

