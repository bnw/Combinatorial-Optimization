#ifndef COMBINATORIAL_OPTIMIZATION_UTILS_H
#define COMBINATORIAL_OPTIMIZATION_UTILS_H

#include <numeric>

/**
 * Creates {0,1,...,n-1}
 */
template<typename Scalar>
std::vector <Scalar> create_0_to_n_minus_one(Scalar const n)
{
	std::vector <Scalar> elements;
	std::iota(elements.begin(), elements.end(), n);
	return elements;
}

#endif //COMBINATORIAL_OPTIMIZATION_UTILS_H
