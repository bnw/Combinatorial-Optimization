#ifndef COMBINATORIAL_OPTIMIZATION_FIXTURES_H
#define COMBINATORIAL_OPTIMIZATION_FIXTURES_H

#include <ShrinkableGraph.h>

struct K4Fixture
{
	K4Fixture()
			: G(4)
	{
		G.add_edge(0, 1);
		G.add_edge(0, 2);
		G.add_edge(0, 3);
		G.add_edge(1, 2);
		G.add_edge(1, 3);
		G.add_edge(2, 3);
	}

	ShrinkableGraph G;
};

struct K5Fixture
{
	K5Fixture()
			: G(5)
	{
		G.add_edge(0, 1);
		G.add_edge(0, 2);
		G.add_edge(0, 3);
		G.add_edge(0, 4);
		G.add_edge(1, 2);
		G.add_edge(1, 3);
		G.add_edge(1, 4);
		G.add_edge(2, 3);
		G.add_edge(2, 4);
		G.add_edge(3, 4);
	}

	ShrinkableGraph G;
};

#endif //COMBINATORIAL_OPTIMIZATION_FIXTURES_H
