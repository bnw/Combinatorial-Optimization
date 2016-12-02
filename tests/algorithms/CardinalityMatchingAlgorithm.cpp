#include <boost/test/unit_test.hpp>
#include <algorithms/CardinalityMatchingAlgorithm.h>
#include "../fixtures.h"

using namespace algorithms;

BOOST_AUTO_TEST_CASE(CardinalityMatchingAlgorithmPath)
{
	ShrinkableGraph G(6);
	G.add_edge(0, 1);
	G.add_edge(0, 2);
	G.add_edge(1, 2);
	G.add_edge(2, 3);
	G.add_edge(3, 4);
	G.add_edge(5, 0);
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(3 == matching.get_num_edges());
}
BOOST_AUTO_TEST_CASE(CardinalityMatchingAlgorithmK1)
{
	ShrinkableGraph G(1);
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(0 == matching.get_num_edges());
}

BOOST_FIXTURE_TEST_CASE(CardinalityMatchingAlgorithmK4, K4Fixture)
{
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(2 == matching.get_num_edges());
}

BOOST_FIXTURE_TEST_CASE(CardinalityMatchingAlgorithmK5, K5Fixture)
{
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(2 == matching.get_num_edges());
}

BOOST_FIXTURE_TEST_CASE(CardinalityMatchingAlgorithmK50, KnFixture<50>)
{
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(25 == matching.get_num_edges());
}

BOOST_AUTO_TEST_CASE(CardinalityMatchingAlgorithmSimple)
{
	ShrinkableGraph G(4);
	G.add_edge(0, 1);
	G.add_edge(0, 2);
	G.add_edge(1, 2);
	G.add_edge(2, 3);
	auto const matching = CardinalityMatchingAlgorithm().run(G);
	BOOST_TEST(2 == matching.get_num_edges());
}