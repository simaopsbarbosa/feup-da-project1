#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

namespace GraphAlgorithms {
    void dijkstra(Graph<LocationInfo>* graph, int source);
    bool relax(Edge<LocationInfo> *edge);
    std::vector<LocationInfo> getPath(Graph<LocationInfo> *g, const int &origin, const int &dest);
}

#endif
