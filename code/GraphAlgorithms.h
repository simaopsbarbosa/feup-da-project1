#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"
#include "data-structures/Path.h"

namespace GraphAlgorithms {

bool                      relax(Edge<LocationInfo> *edge, bool isDriving = true);
std::vector<LocationInfo> dijkstraDriving(Graph<LocationInfo> *graph, int source, const int &dest, const std::vector<int> &avoidNodes,
                                          const std::vector<std::pair<int, int>> &avoidSegments);
std::vector<LocationInfo> dijkstraWalking(Graph<LocationInfo> *graph, int source, const int &dest, const std::vector<int> &avoidNodes,
                                          const std::vector<std::pair<int, int>> &avoidSegments);
std::vector<LocationInfo> restrictedRoute(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                          const std::vector<std::pair<int, int>> &avoidSegments, int includeNode = -1);
Path environmentalRoute(Graph<LocationInfo> *graph, int source, int dest, double maxWalkingTime, const std::vector<int> &avoidNodes,
                        const std::vector<std::pair<int, int>> &avoidSegments);
std::vector<Vertex<LocationInfo> *> getParkingNodes(Graph<LocationInfo> *graph, int source, const std::vector<int> &avoidNodes,
                                                    const std::vector<std::pair<int, int>> &avoidSegments);
} // namespace GraphAlgorithms

#endif
