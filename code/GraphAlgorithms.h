#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

namespace GraphAlgorithms {
void dijkstra(Graph<LocationInfo> *graph, int source,
              bool hasRestrictions = false, const std::vector<int> &avoidNodes = {},
              const std::vector<std::pair<int, int>> &avoidSegments = {});
bool relax(Edge<LocationInfo> *edge);
std::vector<LocationInfo> getPath(Graph<LocationInfo> *g, const int &origin,
                                  const int &dest);
std::vector<LocationInfo> restrictedRoute(
    Graph<LocationInfo> *graph, int source, int dest,
    const std::vector<int> &avoidNodes,
    const std::vector<std::pair<int, int>> &avoidSegments,
    int includeNode = -1);
} // namespace GraphAlgorithms

#endif
