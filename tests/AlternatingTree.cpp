#include <boost/test/unit_test.hpp>
#include <AlternatingTree.h>
#include <set>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(AlternatingTreeEdgeAdd, K4Fixture)
{
	AlternatingTree T(G, 0);
	T.add_edge({0,1});
	BOOST_TEST(T.is_even(0));
	BOOST_TEST(T.is_odd(1));
	BOOST_TEST(T.contains(0));
	BOOST_TEST(T.contains(1));
	BOOST_TEST(not T.contains(2));
	BOOST_TEST(not T.contains(3));
}

BOOST_AUTO_TEST_CASE(FindPath)
{
	ED::Graph G(6);
	G.add_edge(0, 1);
	G.add_edge(0, 2);
	G.add_edge(0, 3);
	G.add_edge(3, 4);
	G.add_edge(4, 5);
	AlternatingTree T(G, 0);
	T.add_edge({0, 1});
	T.add_edge({0, 2});
	T.add_edge({0, 3});
	T.add_edge({3, 4});
	T.add_edge({4, 5});
	auto const path = T.find_path(2, 5);
	std::set<Edge> expected = {{2, 0},
							   {0, 3},
							   {3, 4},
							   {4, 5}};
	std::set<Edge> actual(path.begin(), path.end());
	BOOST_TEST(expected == actual);
	BOOST_TEST(actual.size() == path.size()); //no duplicates
}