#include <set>
#include "UnevenCircuit.h"
#include "ShrinkableGraph.h"

UnevenCircuit::UnevenCircuit(ShrinkableGraph const &graph, std::vector<Edge> const &edges)
		: graph(graph),
		  edges(edges)
{
	assert(edges.size() % 2 == 1);
	assert(edges_are_ordered_correctly());
}

Edge::Vector UnevenCircuit::get_matching_edges_that_expose_one(NodeId const exposed_node_id) const
{
	Edge::Vector matching_edges;
	for (auto edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
		if (edge_it->first_node_id() == exposed_node_id) {
			append(matching_edges, take_every_second(edge_it, edges.end()));
			append(matching_edges,
				   take_every_second(std::reverse_iterator<decltype(edge_it)>(edge_it), edges.rend()));
		}
	}
	assert(matching_edges.size() == edges.size() / 2);
	return matching_edges;
}

Edge::Vector const &UnevenCircuit::get_edges() const
{
	return edges;
}

std::vector<NodeId> UnevenCircuit::get_node_ids() const
{
	std::vector<NodeId> node_ids;
	for (auto const &edge : get_edges()) {
		assert(graph.is_active(edge));
		node_ids.push_back(edge.first_node_id());
	}
	return node_ids;
}

bool UnevenCircuit::operator==(UnevenCircuit const &rhs) const
{
	return edges == rhs.edges;
}

bool UnevenCircuit::edges_are_ordered_correctly() const
{
	//TODO remove debug
	std::set<size_t> visited_partition_classes;


	for (auto edge_it = edges.begin(); edge_it != edges.end() - 1; edge_it++) {
		if (not graph.are_equal(edge_it->second_node_id(), (edge_it + 1)->first_node_id())) {
			return false;
		}
		assert(not visited_partition_classes.count(graph.get_partition_class_id(edge_it->first_node_id())));
		visited_partition_classes.insert(graph.get_partition_class_id(edge_it->first_node_id()));
//		assert(not graph.is_contracted_edge(*edge_it));
	}
	return graph.are_equal(edges.back().second_node_id(), edges.front().first_node_id());
}

template<typename Iterator>
Edge::Vector UnevenCircuit::take_every_second(Iterator iterator, Iterator const &end) const
{
	Edge::Vector result;
	bool is_even = false;
	for (; iterator != end; iterator++, is_even = not is_even) {
		if (is_even) {
			result.push_back(*iterator);
		}
	}
	return result;
}
