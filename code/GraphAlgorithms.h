#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

namespace GraphAlgorithms {
    void dijkstra(Graph<LocationInfo>& graph, int source);
    void bfs(Graph<LocationInfo>& graph, int start);
}

#endif
