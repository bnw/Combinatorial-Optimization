#ifndef COMBINATORIAL_OPTIMIZATION_FIXTURES_H
#define COMBINATORIAL_OPTIMIZATION_FIXTURES_H

#include <ShrinkableGraph.h>


template<size_t n>
struct KnFixture
{
	KnFixture()
			: G(n)
	{
		for (size_t i = 0; i < n; i++) {
			for (size_t j = i + 1; j < n; j++) {
				G.add_edge(i, j);
			}
		}
	}

	ShrinkableGraph G;
};

struct K4Fixture : public KnFixture<4>{};

struct K5Fixture : public KnFixture<5>{};

#endif //COMBINATORIAL_OPTIMIZATION_FIXTURES_H
