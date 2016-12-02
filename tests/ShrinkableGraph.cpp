#include <boost/test/unit_test.hpp>
#include <ShrinkableGraph.h>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(ShrinkingEdge, K4Fixture)
{
	G.shrink_edge(0,1);
	G.shrink_edge(1,2);
	BOOST_TEST(G.is_contracted_edge({0,1}));
	BOOST_TEST(G.is_contracted_edge({1,2}));
	BOOST_TEST(G.is_contracted_edge({0,2}));
	BOOST_TEST(not G.is_contracted_edge({0,3}));
}

BOOST_FIXTURE_TEST_CASE(ShrinkingCircuit, K5Fixture)
{
	OddCircuit circuit_1{G, {{0,1},{1,2},{2,0}}};
	OddCircuit circuit_2{G, {{2,3},{3,4},{4,2}}};
	G.shrink_circuit(circuit_1);
	G.shrink_circuit(circuit_2);
	BOOST_CHECK(circuit_1 == *G.get_shrunken_circuits().at(0));
	BOOST_CHECK(circuit_2 == *G.get_shrunken_circuits().at(1));
	BOOST_TEST(2 == G.get_shrunken_circuits().size());
	BOOST_TEST(G.is_contracted_edge({0,1}));
	BOOST_TEST(G.is_contracted_edge({0,4}));
}