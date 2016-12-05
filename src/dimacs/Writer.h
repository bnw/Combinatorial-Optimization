#ifndef COMBINATORIAL_OPTIMIZATION_WRITER_H
#define COMBINATORIAL_OPTIMIZATION_WRITER_H

#include <ostream>
#include <graph.hpp>
#include <Matching.h>

namespace dimacs {

class Writer
{
public:
	void write(ED::Graph const &graph, Matching const &matching, std::ostream &out) const
	{
		out << "p edge " << graph.num_nodes() << " " << matching.get_num_edges() << std::endl;
		for (auto const &edge : matching.get_edges()) {
			out << "e " << ED::ED_id_to_dimacs_id(edge.first_node_id())
				<< " " << ED::ED_id_to_dimacs_id(edge.second_node_id())
				<< std::endl;
		}
	}
};

}

#endif //COMBINATORIAL_OPTIMIZATION_WRITER_H
