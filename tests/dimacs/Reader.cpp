#include <boost/test/unit_test.hpp>
#include <dimacs/Reader.h>

using namespace dimacs;

BOOST_AUTO_TEST_CASE(SimpleGraph){
	Reader reader;
	std::istringstream input("c comment\np edge 3 2\ne 1 2\ne 2 3");
	std::unique_ptr<ED::Graph> graph;
	BOOST_CHECK_NO_THROW(graph = reader.read(input));
	BOOST_TEST(graph->num_nodes() == 3);
	BOOST_TEST(graph->num_edges() == 2);
	BOOST_TEST(graph->node(0).degree() == 1);
	BOOST_TEST(graph->node(1).degree() == 2);
	BOOST_TEST(graph->node(2).degree() == 1);
}