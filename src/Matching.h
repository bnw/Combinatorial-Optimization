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

	Matching unshrink() const
	{
		Matching M = *this;
		for (auto const &circuit : graph.get_shrunken_circuits()) {
			using namespace std::placeholders;
			auto const covered_node_ids
					= filter<NodeId>(circuit.get_node_ids(), std::bind(&Matching::is_covered, &M, _1));
			assert(covered_node_ids.size() <= 1);
			if(covered_node_ids.size() == 1){
				M.add_edges(circuit.get_matching_edges_that_expose_one(covered_node_ids.at(0)));
			}
		}
		return M;
	}

	void add_edges(Edge::Vector const &edges)
	{
		for (auto const &edge :edges) {
			add_edge(edge);
		}
	}

	void add_edge(Edge const &edge)
	{
		for (auto const node_id : edge.get_node_ids()) {
			assert(not is_covered(node_id));
			assert(covering_edges.at(node_id).is_invalid_edge());
			set_covered(node_id, true);
			covering_edges.at(node_id) = edge;
		}
		num_edges++;
	}

	void remove_edge(Edge const &edge)
	{
		for (auto const node_id : edge.get_node_ids()) {
			assert(is_covered(node_id));
			assert(covering_edges.at(node_id) == edge);
			set_covered(node_id, false);
			covering_edges.at(node_id) = Edge::invalid_edge();
		}
		num_edges--;
		assert(num_edges >= 0);
	}

	void remove_edges(Edge::Vector const &edges)
	{
		for (auto const &edge: edges) {
			remove_edge(edge);
		}
	}

	bool is_covered(NodeId const node_id) const
	{
		return covered_nodes.at(node_id);
	}

	bool is_exposed(NodeId const node_id) const
	{
		return not is_covered(node_id);
	}

	void augment(Edge::Vector const &augmenting_path)
	{
		Edge::Vector new_edges;
		for (auto const &edge : augmenting_path) {
			if (contains_edge(edge)) {
				remove_edge(edge);
			} else {
				new_edges.push_back(edge);
			}
		}
		add_edges(new_edges);
	}

	bool contains_edge(Edge const &edge)
	{
		if (covering_edges.at(edge.first_node_id()) == edge) {
			assert(covering_edges.at(edge.second_node_id()) == edge);
			return true;
		}
		return false;
	}

	NodeId get_one_exposed() const
	{
		for (NodeId node_id = 0; node_id < covered_nodes.size(); node_id++) {
			if (is_exposed(node_id)) {
				return node_id;
			}
		}
		assert(false);
		return 0;
	}

	bool is_perfect() const
	{
		assert(num_edges >= 0);
		return ((size_t) num_edges) * 2 == covered_nodes.size();
	}

	int get_num_edges() const
	{
		return num_edges;
	}

	Edge const &get_covering_edge(NodeId const node_id) const
	{
		assert(is_covered(node_id));
		return covering_edges.at(node_id);
	}

private:
	void set_covered(NodeId const node_id, bool const covered)
	{
		covered_nodes.at(node_id) = covered;
	}

	std::vector<Edge> covering_edges;
	std::vector<bool> covered_nodes;
	int num_edges;
	ShrinkableGraph const &graph;
};


#endif //COMBINATORIAL_OPTIMIZATION_MATCHING_H
