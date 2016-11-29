#include <boost/test/unit_test.hpp>
#include <AlternatingTree.h>
#include <set>
#include "fixtures.h"


BOOST_FIXTURE_TEST_CASE(AlternatingTreeEdgeAdd, K4Fixture)
{
	AlternatingTree T(G, 0);
	T.add_edge({0, 1});
	BOOST_TEST(T.is_even(0));
	BOOST_TEST(T.is_odd(1));
	BOOST_TEST(T.contains_node(0));
	BOOST_TEST(T.contains_node(1));
	BOOST_TEST(not T.contains_node(2));
	BOOST_TEST(not T.contains_node(3));
}

BOOST_AUTO_TEST_CASE(FindPath)
{
	ShrinkableGraph G(6);
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

BOOST_AUTO_TEST_CASE(FindSimplePath)
{
	ShrinkableGraph G(6);
	G.add_edge(0, 1);
	G.add_edge(1, 2);
	AlternatingTree T(G, 0);
	T.add_edge({0, 1});
	T.add_edge({1, 2});
	auto const actual = T.find_path(2, 0);
	std::vector<Edge> expected = {{2, 1},
								  {1, 0},};
	BOOST_TEST(expected == actual);
}

BOOST_AUTO_TEST_CASE(GetNodes)
{
	ShrinkableGraph G(6);
	G.add_edge(0, 1);
	G.add_edge(1, 2);
	AlternatingTree T(G, 0);
	T.add_edge({0, 1});
	T.add_edge({1, 2});
	auto const actual = T.get_nodes();
	std::vector<NodeId> const expected = {0, 1, 2};
	BOOST_TEST(expected == actual);
}