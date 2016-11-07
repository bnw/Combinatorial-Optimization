#ifndef COMBINATORIAL_OPTIMIZATION_READER_H
#define COMBINATORIAL_OPTIMIZATION_READER_H

#include <string>
#include <graph.hpp>
#include <memory>

namespace dimacs {

class Reader
{
public:
	std::unique_ptr<ED::Graph> read(std::istream &input);

private:
	void create_graph(std::unique_ptr<ED::Graph> &graph, std::istringstream &line_stream);

	void create_edge(std::unique_ptr<ED::Graph> const &graph, std::istringstream &line_stream);
};

}


#endif //COMBINATORIAL_OPTIMIZATION_READER_H
