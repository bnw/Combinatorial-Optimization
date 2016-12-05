#ifndef COMBINATORIAL_OPTIMIZATION_MATCHING_H
#define COMBINATORIAL_OPTIMIZATION_MATCHING_H

#include <cassert>
#include <algorithm>
#include <functional>
#include "Edge.h"
#include "typedefs.h"
#include "ShrinkableGraph.h"

class Matching
{
public:
	Matching(ShrinkableGraph const &graph) :
			covering_edges(graph.num_nodes(), Edge::invalid_edge()),
			covered_nodes(graph.num_nodes(), false),
			num_edges(0),
			graph(graph)
	{}

	std::vector<NodeId> get_covered_nodes() const;

	void add_edges(Edge::Vector const &edges);

	void add_edge(Edge const &edge);

	void remove_edge(Edge const &edge);

	void remove_edges_if_contained(Edge::Vector const &edges);

	bool is_covered(NodeId const node_id) const;

	bool is_exposed(NodeId const node_id) const;

	void augment(Edge::Vector const &augmenting_path);

	bool contains_edge(Edge const &edge) const;

	bool has_exposed_vertex() const;

	/**
	 * Returns a node that is exposed and not
	 * deactivated (according to graph.is_active(node_id)).
	 * @return
	 */
	NodeId get_one_exposed() const;

	bool is_perfect() const;

	int get_num_edges() const;

	Edge const &get_covering_edge(NodeId const node_id) const;

	void set_covering_edge(NodeId const node_id, Edge const &edge);

	/**
	 * Not: Could be implemented faster (but is only used for the output).
	 */
	Edge::Set get_edges() const;

private:
	void set_covered(NodeId const node_id, bool const covered);

	std::vector<Edge> covering_edges;
	std::vector<bool> covered_nodes;
	int num_edges;
	ShrinkableGraph const &graph;
};


#endif //COMBINATORIAL_OPTIMIZATION_MATCHING_H
