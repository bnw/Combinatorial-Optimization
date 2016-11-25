#include <boost/test/unit_test.hpp>
#include <utils.h>

BOOST_AUTO_TEST_CASE(createRange)
{
	std::vector<int> const expected = {0, 1, 2, 3, 4};
	auto const actual = create_0_to_n_minus_one(5);
	BOOST_TEST(expected == actual);
}

BOOST_AUTO_TEST_CASE(filter_test)
{
	auto const even = [](int i) { return (i % 2) == 0; };
	std::vector<int> const vector = {0, 1, 2, 3, 4};
	std::vector<int> const expected = {0, 2, 4};
	BOOST_TEST(filter<int>(vector, even) == expected);
}

BOOST_AUTO_TEST_CASE(append_test)
{
	std::vector<int> vector_a = {0, 1, 2};
	std::vector<int> const vector_b = {3, 4};
	std::vector<int> const expected = {0, 1, 2, 3, 4};
	append(vector_a, vector_b);
	BOOST_TEST(vector_a == expected);
}

