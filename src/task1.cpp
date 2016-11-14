#include <iostream>
#include <graph.hpp>
#include <numeric>
#include <utils.h>
#include <set>
#include <cassert>
#include "Matching.h"
#include "AlternatingTree.h"
#include "ShrinkableGraph.h"





int main()
{

	ShrinkableGraph G(1); //TODO read graph from input
	NodeId const r = 0;
	AlternatingTree T(G, r);
	Matching M(G);
	auto L = G.get_incident_edges(r);
	while (not L.empty()) {
		auto const edge = L.back();
		L.pop_back();
		if (G.is_contracted_edge(edge)) {
			continue;
		}
		//TODO Continue algorithm ...
	}


	return EXIT_SUCCESS;
}