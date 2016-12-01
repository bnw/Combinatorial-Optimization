#include <boost/test/unit_test.hpp>
#include <Matching.h>
#include "fixtures.h"

BOOST_FIXTURE_TEST_CASE(AddAndDeleteDuplicate, K4Fixture)
{
	Matching M(G);
	M.add_edge({1, 2});
	BOOST_CHECK(M.contains_edge({1, 2}));
	BOOST_CHECK(not M.contains_edge({1, 3}));
	M.remove_edge({1, 2});
	BOOST_CHECK(M.get_num_edges() == 0);
	BOOST_CHECK(not M.contains_edge({1, 2}));
}

BOOST_FIXTURE_TEST_CASE(GetExposed, K4Fixture)
{
	Matching M(G);
	M.add_edge({0, 1});
	BOOST_CHECK(M.get_one_exposed() == 3 or M.get_one_exposed() == 2);
}

BOOST_FIXTURE_TEST_CASE(IsPerfect, K4Fixture)
{
	Matching M(G);
	M.add_edge({0, 1});
	M.add_edge({2, 3});
	BOOST_CHECK(M.is_perfect());
}

BOOST_FIXTURE_TEST_CASE(IsCovered, K4Fixture)
{
	Matching M(G);
	M.add_edge({0, 1});
	BOOST_CHECK(M.is_covered(0));
	BOOST_CHECK(M.is_covered(1));
	BOOST_CHECK(M.is_exposed(2));
	BOOST_CHECK(M.is_exposed(3));
}

BOOST_FIXTURE_TEST_CASE(Augment, K4Fixture)
{
	Matching M(G);
	M.add_edge({1, 2});
	M.augment({{0, 1},
			   {1, 2},
			   {2, 3}});
	BOOST_CHECK(M.is_perfect());
	BOOST_CHECK(M.is_covered(0));
	BOOST_CHECK(M.is_covered(3));
}

//BOOST_FIXTURE_TEST_CASE(unshrink, K5Fixture)
//{
//	Matching M(G);
//	UnevenCircuit circuit_1{G, {{0,1},{1,2},{2,0}}};
//	G.shrink_circuit(circuit_1);
//	M.add_edge({2,3});
//	auto const M_unshrinked = M.unshrink();
//	BOOST_TEST(2 == M_unshrinked.get_num_edges());
//}