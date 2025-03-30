#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/EnvironmentalPath.h"
#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

namespace GraphAlgorithms {

/**
 * @brief Relaxes an edge during graph traversal.
 *
 * Updates the distance to the destination vertex if a shorter path is found.
 * This function runs in constant time.
 *
 * @param edge The edge to relax.
 * @param isDriving Indicates whether the edge is part of a driving path.
 * @return True if the edge was relaxed, false otherwise.
 */
bool relax(Edge<LocationInfo> *edge, bool isDriving = true);

/**
 * @brief Computes the shortest path between two nodes using Dijkstra's
 * algorithm.
 *
 * Finds the shortest path from the source to the destination while avoiding
 * specified nodes and segments. The algorithm runs in O((V + E) log V), where V
 * is the number of vertices and E is the number of edges.
 *
 * @param graph The graph to traverse.
 * @param source The ID of the source node.
 * @param dest The ID of the destination node.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of edge pairs (source, destination) to avoid.
 * @param isDriving Indicates whether the path is for driving.
 * @return A vector of LocationInfo representing the shortest path.
 */
std::vector<LocationInfo>
dijkstra(Graph<LocationInfo> *graph, int source, int dest,
         const std::vector<int> &avoidNodes,
         const std::vector<std::pair<int, int>> &avoidSegments, bool isDriving);

/**
 * @brief Retrieves the path from the origin to the destination.
 *
 * Constructs the path by backtracking from the destination to the origin using
 * the shortest path tree. This function runs in O(V), where V is the number of
 * vertices in the path.
 *
 * @param g The graph containing the path.
 * @param origin The ID of the origin node.
 * @param dest The ID of the destination node.
 * @param isDriving Indicates whether the path is for driving.
 * @return A vector of LocationInfo representing the path.
 */
std::vector<LocationInfo> getPath(Graph<LocationInfo> *g, const int &origin,
                                  const int &dest, bool isDriving);

/**
 * @brief Computes the driving route between two nodes.
 *
 * Finds the shortest driving route from the source to the destination,
 * optionally passing through an intermediate node. The algorithm runs in O((V +
 * E) log V) for each segment of the route (source to intermediate and
 * intermediate to destination).
 *
 * @param graph The graph to traverse.
 * @param source The ID of the source node.
 * @param dest The ID of the destination node.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of edge pairs (source, destination) to avoid.
 * @param includeNode An optional intermediate node to include in the route.
 * @return A vector of LocationInfo representing the driving route.
 */
std::vector<LocationInfo>
drivingRoute(Graph<LocationInfo> *graph, int source, int dest,
             const std::vector<int> &avoidNodes,
             const std::vector<std::pair<int, int>> &avoidSegments,
             int includeNode = -1);

/**
 * @brief Finds all parking nodes reachable from the source.
 *
 * Identifies parking nodes and computes the driving path to each of them.
 * The algorithm runs in O((V + E) log V), where V is the number of vertices and
 * E is the number of edges.
 *
 * @param graph The graph to traverse.
 * @param source The ID of the source node.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of edge pairs (source, destination) to avoid.
 * @return A vector of pairs, each containing a parking node and the path to it.
 */
std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>>
getParkingNodes(Graph<LocationInfo> *graph, int source,
                const std::vector<int> &avoidNodes,
                const std::vector<std::pair<int, int>> &avoidSegments);

/**
 * @brief Computes an environmental route combining driving and walking.
 *
 * Finds a route from the source to the destination that minimizes total time
 * while respecting a maximum walking time. The algorithm runs in O(V⋅(V+E)logV).
 *
 * @param graph The graph to traverse.
 * @param source The ID of the source node.
 * @param dest The ID of the destination node.
 * @param maxWalkingTime The maximum allowable walking time.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of edge pairs (source, destination) to avoid.
 * @return A vector of EnvironmentalPath objects representing the possible
 * routes.
 */
std::vector<EnvironmentalPath>
drivingWalkingRoute(Graph<LocationInfo> *graph, int source, int dest,
                   double maxWalkingTime, const std::vector<int> &avoidNodes,
                   const std::vector<std::pair<int, int>> &avoidSegments);

} // namespace GraphAlgorithms

#endif // GRAPH_ALGORITHMS_H
