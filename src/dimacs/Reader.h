#ifndef COMBINATORIAL_OPTIMIZATION_READER_H
#define COMBINATORIAL_OPTIMIZATION_READER_H

#include <string>
#include <graph.hpp>
#include <memory>
#include <ShrinkableGraph.h>

namespace dimacs {

class Reader
{
public:
	std::unique_ptr<ShrinkableGraph> read(std::istream &input) const;

private:
	void create_graph(std::unique_ptr<ShrinkableGraph> &graph, std::istringstream &line_stream) const;

	void create_edge(std::unique_ptr<ShrinkableGraph> const &graph, std::istringstream &line_stream) const;
};

}


#endif //COMBINATORIAL_OPTIMIZATION_READER_H
