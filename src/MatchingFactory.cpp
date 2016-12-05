#include "MatchingFactory.h"

Matching MatchingFactory::create_from_subgraph(ShrinkableGraph const &graph, ED::Graph const &subgraph) const
{
	Matching matching(graph);
	for (NodeId node_id = 0; node_id < subgraph.num_nodes(); node_id++) {
		for (auto neighbor_node_id : subgraph.node(node_id).neighbors()) {
			if (neighbor_node_id > node_id) {
				matching.add_edge({neighbor_node_id, node_id});
			}
		}
	}
	return matching;
}
