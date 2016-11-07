#include <iostream>
#include <graph.hpp>

int main()
{
	ED::Graph graph(2);
	graph.add_edge(0,1);

	std::cout << "Hello, World!" << graph.node(0).degree() << std::endl;
	return 0;
}