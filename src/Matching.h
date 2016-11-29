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
		//TODO remove debug
		for(auto & circuit : graph.get_shrunken_circuits()){
			using namespace std::placeholders;
			auto const covered_node_ids
					= filter<NodeId>(circuit.get_node_ids(), std::bind(&Matching::is_covered, this, _1));
			assert(covered_node_ids.size() < 2);
		}


		Matching M = *this;
		for (auto const &circuit : graph.get_shrunken_circuits()) {
			using namespace std::placeholders;

			//TODO remove debug
			for (auto const node: circuit.get_node_ids()) {
				assert(graph.is_active(node));
			}
			for(auto &edge : circuit.get_edges()){
				assert(graph.is_contracted_edge(edge));
			}

			auto const covered_node_ids
					= filter<NodeId>(circuit.get_node_ids(), std::bind(&Matching::is_covered, &M, _1));

			//TODO remove debug
			for(auto n : covered_node_ids){
				assert(M.is_covered(n));
			}

			assert(covered_node_ids.size() <= 1);
			NodeId exposed_node;
			if (covered_node_ids.size() == 1) {
				exposed_node = covered_node_ids.at(0);
			} else {
				exposed_node = circuit.get_node_ids().front();
			}
			M.add_edges(circuit.get_matching_edges_that_expose_one(exposed_node));
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
		assert(graph.is_active(edge));
		for (auto const node_id : edge.get_node_ids()) {
			assert(not is_covered(node_id));
			assert(get_covering_edge(node_id).is_invalid_edge());
			set_covered(node_id, true);
			set_covering_edge(node_id, edge);
		}
		num_edges++;
	}

	void remove_edge(Edge const &edge)
	{
		assert(graph.is_active(edge));
		for (auto const node_id : edge.get_node_ids()) {
			assert(is_covered(node_id));
			assert(get_covering_edge(node_id) == edge);
			set_covered(node_id, false);
			set_covering_edge(node_id, Edge::invalid_edge());
		}
		num_edges--;
		assert(num_edges >= 0);
	}

	void remove_edges_if_contained(Edge::Vector const &edges)
	{
		//TODO remove debug
		int num_removed = 0;

		for (auto const &edge: edges) {
			assert(graph.is_active(edge));
			if (contains_edge(edge)) {
				remove_edge(edge);
				num_removed++;
			}
		}

		assert(edges.size() / 2 == num_removed);
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
			assert(graph.is_active(edge));
			assert(not graph.is_contracted_edge(edge));
			if (contains_edge(edge)) {
				remove_edge(edge);
			} else if (not graph.is_contracted_edge(edge)) {
				new_edges.push_back(edge);
			}
		}
		assert(new_edges.size() == augmenting_path.size() / 2 + 1);
		add_edges(new_edges);

		//TODO remove debug
		for(auto &circuit : graph.get_shrunken_circuits()){
			int num_covered = 0;
			for(auto node : circuit.get_node_ids()){
				if(is_covered(node)){
					num_covered++;
				}
			}
			assert(num_covered <= 1);
		}
	}

	bool contains_edge(Edge const &edge)
	{
		if (covering_edges.at(edge.first_node_id()) == edge) {
			assert(covering_edges.at(edge.second_node_id()) == edge);
			return true;
		}
		return false;
	}

	bool has_exposed_vertex() const
	{
		for (NodeId node_id = 0; node_id < graph.num_nodes(); node_id++) {
			if (is_exposed(node_id) and graph.is_active(node_id)) {
				return true;
			}
		}
		return false;
	}

	//TODO runtime?
	NodeId get_one_exposed() const
	{
		for (NodeId node_id = 0; node_id < graph.num_nodes(); node_id++) {
			if (is_exposed(node_id) and graph.is_active(node_id)) {
				assert(not graph.is_pseudo_node(node_id));
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
		return covering_edges.at(node_id);
	}

	void set_covering_edge(NodeId const node_id, Edge const &edge)
	{
		covering_edges.at(node_id) = edge;
	}

	Matching &operator=(Matching const &other)
	{
		covering_edges = other.covering_edges;
		covered_nodes = other.covered_nodes;
		num_edges = other.num_edges;
		assert(&graph == &other.graph);
		return *this;
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
