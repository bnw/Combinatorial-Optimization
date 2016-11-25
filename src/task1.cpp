#include <iostream>
#include <fstream>
#include <graph.hpp>
#include <numeric>
#include <utils.h>
#include <set>
#include <cassert>
#include <memory>
#include <dimacs/Reader.h>
#include "Matching.h"
#include "AlternatingTree.h"
#include "ShrinkableGraph.h"
#include "utils.h"
#include "UnevenCircuit.h"


int main(int argc, const char *argv[])
{

	dimacs::Reader const reader{};
	std::ifstream file(argv[1]);
	auto G = reader.read(file);
	file.close();
	NodeId const r = 0;
	std::unique_ptr<AlternatingTree> T(new AlternatingTree(*G, r));
	Matching M(*G);
	auto L = G->get_incident_edges(r);
	while (not L.empty()) {
		auto const edge = L.back();
		L.pop_back();
		if (G->is_contracted_edge(edge)) {
			continue;
		}
		auto even_vertices = filter<NodeId>(edge.get_node_ids(), [&](NodeId n) { return T->is_even(n); });
		if (even_vertices.size() == 2) {
			auto circuit_edges = T->find_path(even_vertices.at(0), even_vertices.at(1));
			circuit_edges.push_back({even_vertices.at(1), even_vertices.at(0)});
			G->shrink_circuit(UnevenCircuit(*G, circuit_edges));
			for (auto const odd_node_id : T->get_odd_nodes()) {
				append(L, G->get_incident_edges(odd_node_id));
			}
			//TODO update T (?)
		} else if (even_vertices.size() == 1) {
			auto const even_vertex = even_vertices.at(0);
			auto const other_vertex = edge.other_vertex(even_vertex);
			if (M.is_exposed(other_vertex)) {
				auto augmenting_path = T->find_path_from_root_to_node(even_vertex);
				augmenting_path.push_back(edge);
				M.augment(augmenting_path);
				auto const M_unshrinked = M.unshrink();
				if (M_unshrinked.is_perfect()) {
					std::cout << "Found perfect matching with " << M_unshrinked.get_num_edges() << " edges!" << std::endl;
					break;
				}
				T = std::unique_ptr<AlternatingTree>{new AlternatingTree(*G, M.get_one_exposed())};
			} else {
				assert(M.is_covered(other_vertex));
				T->add_edge(edge);
				auto const covering_edge = M.get_covering_edge(other_vertex);
				auto const z = covering_edge.other_vertex(other_vertex);
				T->add_edge(covering_edge);
				append(L, G->get_incident_edges(z));
			}
		}
		std::cout << "Size of L: " << L.size() << ", size of M: " << M.get_num_edges() << std::endl;
	}

	std::cout << "Cardinality of maximum matching is " << M.unshrink().get_num_edges() << std::endl;


	return EXIT_SUCCESS;
}