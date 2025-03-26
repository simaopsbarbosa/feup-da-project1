#include "Environment.h"
#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <unordered_map>

bool Environment::is_adjacent(Graph<LocationInfo>* graph, const int &origin, const int &dest) {
    auto origin_vertex = graph->findVertexById(origin);
    auto dest_vertex = graph->findVertexById(dest);
    for (auto& edge : origin_vertex->getAdj()) {
        if (edge->getDest() == dest_vertex) {
            return true;
        }
    }
    return false;
}

bool Environment::is_parking_node(Graph<LocationInfo>* graph, const int &node) {
    auto vertex = graph->findVertexById(node);
    return vertex->getInfo().parking;
}

std::vector<Vertex<LocationInfo>*> Environment::get_parking_nodes(Graph<LocationInfo>* graph) {
    std::vector<Vertex<LocationInfo>*> parkingNodes;
    for (auto& vertex : graph->getVertexSet()) {
        if (vertex->getInfo().parking) {
            parkingNodes.push_back(vertex);
        }
    }
    return parkingNodes;
}

bool Environment::relax(Edge<LocationInfo> *edge, bool isWalking) { // d[u] + w(u,v) < d[v]
     auto dist = (isWalking) ? edge->getWalkingWeight() : edge->getDrivingWeight();
     auto originDist = (isWalking) ? edge->getOrig()->getWalkingDist() : edge->getOrig()->getDrivingDist();
     auto destDist = (isWalking) ? edge->getDest()->getWalkingDist() : edge->getDest()->getDrivingDist();
     if (originDist + dist < destDist) {
         if (isWalking) {
             edge->getDest()->setWalkingDist(originDist + dist);
         } else {
             edge->getDest()->setDrivingDist(originDist + dist);
         }
         edge->getDest()->setPath(edge);
         return true;
     }
     return false;
}

void Environment::dijkstra(Graph<LocationInfo>* graph, int source) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;

    for (auto& vertex : graph->getVertexSet()) {
        vertex->setDrivingDist(INF);
        vertex->setWalkingDist(INF);
        vertex->setPath(nullptr);
    }

    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return;
    }

    sourceVertex->setDrivingDist(0);
    sourceVertex->setWalkingDist(0);

    pq.insert(sourceVertex);

    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();

        for (auto& edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();
            bool relaxed = false;

            if (edge->getWalkingWeight() > 0) { // não sei se podem ser estas as condições
              relaxed = relax(edge, true);
            } else if (edge->getDrivingWeight() > 0) {
              relaxed = relax(edge, false);
            }

            if (relaxed) {
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }
 }

std::vector<LocationInfo> Environment::getPath(Graph<LocationInfo> *graph, const int &origin, const int &dest, double maxWalkingTime) {
    std::vector<LocationInfo> result;

    if (is_adjacent(graph, origin, dest)) {
        std::cerr << "Origin and destination cannot be adjacent nodes.\n";
        return result;
    }

    if (is_parking_node(graph, origin) || is_parking_node(graph, dest)) {
        std::cerr << "Origin and destination cannot be parking nodes.\n";
        return result;
    }

    std::vector<Vertex<LocationInfo>*> parkingNodes = get_parking_nodes(graph);
    std::vector<Vertex<LocationInfo>*> validParkingNodes;
    std::vector<std::tuple<Vertex<LocationInfo>*, double, double, double>> validRoutes;
    std::unordered_map<int,double> dist;

    dijkstra(graph, origin);

    for (auto& parkingNode : parkingNodes) {
      if (parkingNode->getDrivingDist() != INF) {
          validParkingNodes.push_back(parkingNode);
      }
    }

    for (auto& validNode : validParkingNodes) {
      dijkstra(graph, validNode->getInfo().id);
      double walkingDist = validNode->getWalkingDist();
      if (walkingDist <= maxWalkingTime) {
        double totalTime = validNode->getDrivingDist() + walkingDist;
        validRoutes.push_back(std::make_tuple(validNode, validNode->getDrivingDist(), totalTime, walkingDist));
      }
    }

    if (!validRoutes.empty()) {
        auto bestRoute = std::min_element(validRoutes.begin(), validRoutes.end(),
              [](const auto& a, const auto& b) {
                  return std::get<2>(a) < std::get<2>(b) ||
                  ((std::get<2>(a) == std::get<2>(b)) && (std::get<3>(a) < std::get<3>(b)));
        });

        auto selectedNode = std::get<0>(*bestRoute);
        result.push_back(selectedNode->getInfo());

    } else {
        std::cerr << "[ERROR] No valid route found.\n";
        return result;
    }

    return result;
}




// STEPS
// 1º - if origin and destination are adjacent nodes, return "origin and destination cannot be adjacent nodes"
// 2º - if origin or destination are parking nodes, return "origin and destination cannot be parking nodes"
// 3º - for each parking node P, compute shortest route from origin to P (dijkstra)
// 4º - store node and driving time if the route is valid
// 5º - "remove" parking nodes without a valid route from origin
// 6º - for each parking node P found in step 3, compute shortest route from P to destination (dijkstra) (pedestrian paths only)
// 7º - store node and walking time if walking time is equal or less than maximum walking time
// 8º - for each route sum driving time and walking time
// 9º - choose route with shortest driving + walking time
// 10º - if there is a tie, choose route with the longest walking time
// 11º - if valid route is found, return it
// 12º - else, indicate issue ( walking time exceeds predefined maximum limit or absence of parking, or both)

// PSEUDO CODE +/-
/*
environmental_route (graph, origin, destination, max_walking) {
    P = get_parking_nodes(graph)
    if is_ajacent(graph, origin, destination) return "origin and destination cannot be adjacent nodes"
    if is_parking_node(graph, origin) or is_parking_node(graph, destination) return "origin and destination cannot be parking nodes"
    valid_routes = []
    driving_distances = dijkstra(graph, origin, walking=0)
    for each parking node p in P {
        if driving_distances[p] is valid then valid_parking_nodes.add(p);
    }
    for each parking node p in valid_parking_nodes {
        walking_distances = dijkstra(graph, p, walking=1)
        if walking_distances[destination] <= max_walking then
            total_time = driving_distances[p] + walking_distances[destination]
            valid_routes.append((p, driving_distances[p], walking_distances[destination], total_time))
    }
    if routes empty return no valid route found // indicate the issue also
    bestroute = min(routes, key=(total time, -walking time))
    return bestroute
*/