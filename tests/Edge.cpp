#include <boost/test/unit_test.hpp>
#include <Edge.h>

BOOST_AUTO_TEST_CASE(EdgeRelations)
{
	Edge a(1, 2), b(2, 1);
	BOOST_CHECK(a == a);
	BOOST_CHECK(b == a);
	BOOST_CHECK(a == b);
	BOOST_CHECK(b == b);
	BOOST_CHECK(not (a < b));
	BOOST_CHECK(not (b < a));
}

BOOST_AUTO_TEST_CASE(EdgeOtherVertex)
{
	Edge a(1, 2);
	BOOST_CHECK(a.other_vertex(1) == 2);
	BOOST_CHECK(a.other_vertex(2) == 1);
}
BOOST_AUTO_TEST_CASE(EdgeReverse)
{
	Edge a(1, 2);
	Edge a_reverse = a.reverse();
	BOOST_CHECK(a.first_node_id() == a_reverse.second_node_id());
	BOOST_CHECK(a_reverse.first_node_id() == a.second_node_id());
}