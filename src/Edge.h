#ifndef COMBINATORIAL_OPTIMIZATION_EDGE_H
#define COMBINATORIAL_OPTIMIZATION_EDGE_H

#include <vector>
#include <list>
#include <graph.hpp>
#include <utility>
#include <cassert>
#include <set>
#include <array>
#include "typedefs.h"

class Edge
{
public:
	using Vector = std::vector<Edge>;
	using List = std::list<Edge>;
	using Set = std::set<Edge>;

	Edge(NodeId const first_node_id, NodeId const second_node_id);

	Edge reverse() const;

	static Edge invalid_edge();

	bool is_invalid_edge() const;

	std::vector<NodeId> get_node_ids() const;

	NodeId first_node_id() const;

	NodeId second_node_id() const;

	bool operator==(Edge const &rhs) const;

	bool operator!=(Edge const &rhs) const;

	bool operator<(Edge const &rhs) const;

	NodeId other_vertex(NodeId const node_id) const;

private:
	std::pair<NodeId, NodeId> node_ids;
};


#endif //COMBINATORIAL_OPTIMIZATION_EDGE_H
