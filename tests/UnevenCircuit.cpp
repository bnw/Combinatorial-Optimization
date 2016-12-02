#include <boost/test/unit_test.hpp>
#include <OddCircuit.h>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(get_matching_edges_that_expose_one, K5Fixture)
{
	OddCircuit uneven_circle(G, {{0, 1},
									{1, 2},
									{2, 3},
									{3, 4},
									{4, 0}});
	auto actual = uneven_circle.get_matching_edges_that_expose_one(3);
	decltype(actual) expected = {{4,0},{1,2}};
	BOOST_TEST(expected == actual);

	actual = uneven_circle.get_matching_edges_that_expose_one(4);
	expected = {{3,2},{1,0}};
	BOOST_TEST(expected == actual);
}

BOOST_FIXTURE_TEST_CASE(get_matching_edges_that_expose_one_2, K5Fixture)
{
	OddCircuit uneven_circle(G, {{0, 1},
									{1, 2},
									{2, 0}});
	auto const actual = uneven_circle.get_matching_edges_that_expose_one(2);
	decltype(actual) expected = {{0,1}};
	BOOST_TEST(expected == actual);
}

BOOST_FIXTURE_TEST_CASE(get_node_ids, K5Fixture)
{
	OddCircuit uneven_circle(G, {{0, 1},
									{1, 2},
									{2, 0}});
	auto const actual = uneven_circle.get_node_ids();
	decltype(actual) expected = {0,1,2};
	BOOST_TEST(expected == actual);
}