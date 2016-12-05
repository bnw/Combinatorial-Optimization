#ifndef COMBINATORIAL_OPTIMIZATION_MATCHINGFACTORY_H
#define COMBINATORIAL_OPTIMIZATION_MATCHINGFACTORY_H


#include "Matching.h"

class MatchingFactory
{
public:
	Matching create_from_subgraph(ShrinkableGraph const &graph, ED::Graph const &subgraph) const;
};


#endif //COMBINATORIAL_OPTIMIZATION_MATCHINGFACTORY_H
