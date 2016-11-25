#include <boost/test/unit_test.hpp>
#include <UnevenCircuit.h>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(get_matching_edges_that_expose_one, K5Fixture)
{
	UnevenCircuit uneven_circle(G, {{0, 1},
								   {1, 2},
								   {2, 3},
								   {3, 4},
								   {4, 0}});
	auto const actual = uneven_circle.get_matching_edges_that_expose_one(3);
	decltype(actual) expected = {{4,0},{1,2}};
	BOOST_TEST(expected == actual);
}