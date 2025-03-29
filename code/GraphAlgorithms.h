/**
 * @file GraphAlgorithms.h
 * @brief Contains graph-based algorithms for route planning, including driving,
 * walking, and environmental routes.
 */

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "data-structures/EnvironmentalPath.h"
#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

/**
 * @namespace GraphAlgorithms
 * @brief Provides a collection of graph-based algorithms for route planning.
 */
namespace GraphAlgorithms {

/**
 * @brief Attempts to relax an edge to improve distance estimates.
 * @param edge Pointer to the edge to be relaxed.
 * @param isDriving Indicates whether the relaxation is for driving (true) or
 * walking (false). Defaults to true.
 * @return True if the relaxation was successful, false otherwise.
 */
bool relax(Edge<LocationInfo> *edge, bool isDriving = true);

/**
 * @brief Retrieves the shortest path from the origin to the destination.
 * @param g Pointer to the graph.
 * @param origin The ID of the origin node.
 * @param dest The ID of the destination node.
 * @param isDriving True for driving mode, false for walking.
 * @return A vector of LocationInfo representing the shortest path.
 */
std::vector<LocationInfo> getPath(Graph<LocationInfo> *g, const int &origin,
                                  const int &dest, bool isDriving);

/**
 * @brief Computes the shortest driving route using Dijkstra's algorithm.
 * @param graph Pointer to the graph.
 * @param source The source node ID.
 * @param dest The destination node ID.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of road segments (pairs of node IDs) to avoid.
 * @return A vector of LocationInfo representing the optimal driving route.
 */
std::vector<LocationInfo>
dijkstraDriving(Graph<LocationInfo> *graph, int source, int dest,
                const std::vector<int> &avoidNodes,
                const std::vector<std::pair<int, int>> &avoidSegments);

/**
 * @brief Computes the shortest walking route using Dijkstra's algorithm.
 * @param graph Pointer to the graph.
 * @param source The source node ID.
 * @param dest The destination node ID.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of road segments (pairs of node IDs) to avoid.
 * @return A vector of LocationInfo representing the shortest walking route.
 */
std::vector<LocationInfo>
dijkstraWalking(Graph<LocationInfo> *graph, int source, int dest,
                const std::vector<int> &avoidNodes,
                const std::vector<std::pair<int, int>> &avoidSegments);

/**
 * @brief Computes a normal route considering user constraints and optional
 * inclusion of a specific node.
 * @param graph Pointer to the graph.
 * @param source The source node ID.
 * @param dest The destination node ID.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of road segments (pairs of node IDs) to avoid.
 * @param includeNode An optional node to include in the route (-1 means no
 * specific inclusion).
 * @return A vector of LocationInfo representing the computed route.
 */
std::vector<LocationInfo>
normalRoute(Graph<LocationInfo> *graph, int source, int dest,
            const std::vector<int> &avoidNodes,
            const std::vector<std::pair<int, int>> &avoidSegments,
            int includeNode = -1);

/**
 * @brief Computes an environmentally-friendly route combining driving and
 * walking.
 * @param graph Pointer to the graph.
 * @param source The source node ID.
 * @param dest The destination node ID.
 * @param maxWalkingTime The maximum allowable walking time.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of road segments (pairs of node IDs) to avoid.
 * @return A list of EnvironmentalPath objects containing possible routes.
 */
std::vector<EnvironmentalPath>
environmentalRoute(Graph<LocationInfo> *graph, int source, int dest,
                   double maxWalkingTime, const std::vector<int> &avoidNodes,
                   const std::vector<std::pair<int, int>> &avoidSegments);

/**
 * @brief Finds suitable parking nodes near the destination to optimize the
 * environmental route.
 * @param graph Pointer to the graph.
 * @param source The source node ID.
 * @param avoidNodes A list of node IDs to avoid.
 * @param avoidSegments A list of road segments (pairs of node IDs) to avoid.
 * @return A vector of pairs, each containing a parking node and its
 * corresponding driving path.
 */
std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>>
getParkingNodes(Graph<LocationInfo> *graph, int source,
                const std::vector<int> &avoidNodes,
                const std::vector<std::pair<int, int>> &avoidSegments);

} // namespace GraphAlgorithms

#endif // GRAPH_ALGORITHMS_H
