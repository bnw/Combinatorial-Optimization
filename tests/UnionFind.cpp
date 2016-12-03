#include <boost/test/unit_test.hpp>
#include <UnionFind.h>

BOOST_AUTO_TEST_CASE(test_union)
{
	UnionFind union_find(4);
	for(size_t i = 0; i < 4; i++){
		BOOST_TEST(i == union_find.find(i));
	}
	union_find.join(0, 1);
	BOOST_TEST(union_find.find(0) == union_find.find(1));
	BOOST_TEST(union_find.find(1) != union_find.find(2));
	BOOST_TEST(union_find.find(1) != union_find.find(3));
	BOOST_TEST(union_find.find(2) != union_find.find(3));
	union_find.join(0, 2);
	BOOST_TEST(union_find.find(1) == union_find.find(2));
	BOOST_TEST(union_find.find(0) == union_find.find(2));
	BOOST_TEST(union_find.find(2) != union_find.find(3));
}