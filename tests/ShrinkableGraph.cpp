#include <boost/test/unit_test.hpp>
#include <ShrinkableGraph.h>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(Shrinking, K4Fixture)
{
	G.shrink_edge(0,1);
	G.shrink_edge(1,2);
	BOOST_TEST(G.is_contracted_edge({0,1}));
	BOOST_TEST(G.is_contracted_edge({1,2}));
	BOOST_TEST(G.is_contracted_edge({0,2}));
	BOOST_TEST(not G.is_contracted_edge({0,3}));
}