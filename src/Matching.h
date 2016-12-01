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

	void unshrink()
	{
		Matching M = *this;

		auto const num_circuits = graph.get_shrunken_circuits().size();
		std::vector<NodeId> nodes_to_expand = get_covered_nodes();
		std::vector<bool> circuit_already_expanded(num_circuits, false);

		auto const expand_outermost_circuit = [&]() {
			for (int circuit_id = (int) num_circuits - 1; circuit_id >= 0; circuit_id--) {
				if (circuit_already_expanded.at((size_t) circuit_id)) {
					continue;
				}
				auto const &circuit = *graph.get_shrunken_circuits().at((size_t) circuit_id);
				assert(circuit.get_id() == circuit_id);
				circuit_already_expanded.at(circuit.get_id()) = true;
				auto const some_circuit_node = circuit.get_node_ids().front();
				auto const new_matching_edges = circuit.get_matching_edges_that_expose_one(some_circuit_node);
				add_edges(new_matching_edges);
				for(auto const& edge : new_matching_edges){
					append(nodes_to_expand, edge.get_node_ids());
				}
				assert(not nodes_to_expand.empty());
				break;
			}
		};

		//TODO evtl geht das auch außerhalb des loops
		if (nodes_to_expand.empty()) {
			expand_outermost_circuit();
			assert(not nodes_to_expand.empty());
		}

		while (not nodes_to_expand.empty()) {
			auto const node_id = nodes_to_expand.back();
			nodes_to_expand.pop_back();
			for(auto const &circuit : graph.get_circuits(node_id)) {
				if (circuit_already_expanded.at(circuit->get_id())) {
					continue;
				}
				circuit_already_expanded.at(circuit->get_id()) = true;
				auto const new_matching_edges = circuit->get_matching_edges_that_expose_one(node_id);
				add_edges(new_matching_edges);
				for(auto const& edge : new_matching_edges){
					append(nodes_to_expand, edge.get_node_ids());
				}
			}
			//TODO evtl geht das auch außerhalb des loops
			if (nodes_to_expand.empty()) {
				expand_outermost_circuit();
			}
		}

		for(auto has_been_expanded : circuit_already_expanded){
			assert(has_been_expanded);
		}
	}

	//TODO write with filter
	Edge::Vector get_edges() const
	{
		Edge::Vector edges;
		for (auto const &edge : graph.get_edges()) {
			if (contains_edge(edge)) {
				edges.push_back(edge);
			}
		}
		return edges;
	}

	std::vector<NodeId> get_covered_nodes() const
	{
		std::vector<NodeId> covered_nodes;
		for (NodeId node_id = 0; node_id < graph.num_nodes(); node_id++) {
			if (is_covered(node_id)) {
				covered_nodes.push_back(node_id);
			}
		}
		return covered_nodes;
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
		for (auto const &edge: edges) {
			assert(graph.is_active(edge));
			if (contains_edge(edge)) {
				remove_edge(edge);
			}
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
			assert(graph.is_active(edge));
			if (contains_edge(edge)) {
				remove_edge(edge);
			} else {
				new_edges.push_back(edge);
			}
		}
		assert(new_edges.size() == augmenting_path.size() / 2 + 1);
		add_edges(new_edges);

	}

	bool contains_edge(Edge const &edge) const
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
