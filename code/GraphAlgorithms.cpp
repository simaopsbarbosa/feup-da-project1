#include "GraphAlgorithms.h"

#include <iostream>
#include <queue>

bool GraphAlgorithms::relax(Edge<LocationInfo> *edge, bool isDriving) {
    if (!edge || !edge->getOrig() || !edge->getDest()) {
        return false;
    }

    double currentDist = isDriving ? edge->getOrig()->getDrivingDist() : edge->getOrig()->getWalkingDist();
    double edgeWeight  = isDriving ? edge->getDrivingWeight() : edge->getWalkingWeight();
    double newDist     = currentDist + edgeWeight;

    if (newDist < (isDriving ? edge->getDest()->getDrivingDist() : edge->getDest()->getWalkingDist())) {
        if (isDriving) {
            edge->getDest()->setDrivingDist(newDist);
        } else {
            edge->getDest()->setWalkingDist(newDist);
        }
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

std::vector<LocationInfo> GraphAlgorithms::getPath(Graph<LocationInfo> *g, const int &origin, const int &dest, bool isDriving) {
    if ((isDriving ? g->findVertexById(dest)->getDrivingDist() : g->findVertexById(dest)->getWalkingDist()) == INF) {
        // destination is unreachable from the origin
        return {};
    }

    std::vector<LocationInfo> res;

    auto currentVertex = g->findVertexById(dest);
    if (!currentVertex) {
        std::cerr << "[ERROR] Destination node not found in the graph.\n";
        return res;
    }

    while (currentVertex != nullptr && currentVertex->getPath() != nullptr) {
        res.push_back(currentVertex->getInfo());
        currentVertex = currentVertex->getPath()->getOrig();
        if (currentVertex->getInfo().id != origin && currentVertex->getInfo().id != dest) {
            currentVertex->setVisited(true);
        }
    }

    if (currentVertex != nullptr && currentVertex->getInfo().id == origin) {
        res.push_back(currentVertex->getInfo());
    } else {
        // no path found from origin to destination
        return {};
    }

    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<LocationInfo> GraphAlgorithms::dijkstraDriving(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;
    std::vector<LocationInfo>                  path;

    for (auto &vertex : graph->getVertexSet()) {
        vertex->setDrivingDist(INF);
        vertex->setPath(nullptr);
        vertex->setQueueIndex(0);
    }

    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return path;
    }

    sourceVertex->setDrivingDist(0);
    pq.insert(sourceVertex);

    if (graph->findVertexById(dest) == nullptr) {
        std::cerr << "[ERROR] Destination node not found in the graph.\n";
        return {};
    }

    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();

        if (currentVertex->getInfo().id == dest) {
            while (currentVertex != nullptr) {
                path.push_back(currentVertex->getInfo());
                currentVertex = currentVertex->getPath() ? currentVertex->getPath()->getOrig() : nullptr;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (!avoidNodes.empty() && std::find(avoidNodes.begin(), avoidNodes.end(), currentVertex->getInfo().id) != avoidNodes.end()) {
            continue;
        }

        for (auto &edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();

            if (!avoidSegments.empty() && std::find(avoidSegments.begin(), avoidSegments.end(),
                                                    std::make_pair(currentVertex->getInfo().id, neighbor->getInfo().id)) != avoidSegments.end()) {
                continue;
            }

            if (relax(edge, true)) {
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }

    return {};
}

std::vector<LocationInfo> GraphAlgorithms::dijkstraWalking(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;
    std::vector<LocationInfo>                  path;

    for (auto &vertex : graph->getVertexSet()) {
        vertex->setWalkingDist(INF);
        vertex->setPath(nullptr);
        vertex->setQueueIndex(0);
    }

    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return path;
    }

    sourceVertex->setWalkingDist(0);
    pq.insert(sourceVertex);

    if (graph->findVertexById(dest) == nullptr) {
        std::cerr << "[ERROR] Destination node not found in the graph.\n";
        return {};
    }

    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();

        if (currentVertex->getInfo().id == dest) {
            while (currentVertex != nullptr) {
                path.push_back(currentVertex->getInfo());
                currentVertex = currentVertex->getPath() ? currentVertex->getPath()->getOrig() : nullptr;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (!avoidNodes.empty() && std::find(avoidNodes.begin(), avoidNodes.end(), currentVertex->getInfo().id) != avoidNodes.end()) {
            continue;
        }

        for (auto &edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();

            if (!avoidSegments.empty() && std::find(avoidSegments.begin(), avoidSegments.end(),
                                                    std::make_pair(currentVertex->getInfo().id, neighbor->getInfo().id)) != avoidSegments.end()) {
                continue;
            }

            if (relax(edge, false)) {
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }

    return {};
}

std::vector<LocationInfo> GraphAlgorithms::normalRoute(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                                       const std::vector<std::pair<int, int>> &avoidSegments, int includeNode) {

    if (source == dest) {
        // source node equal to destination node
        return {};
    }

    std::vector<LocationInfo> fullPath;
    if (includeNode != -1) {
        std::vector<LocationInfo> pathToInclude = dijkstraDriving(graph, source, includeNode, avoidNodes, avoidSegments);
        if (pathToInclude.empty()) {
            return {};
        }
        std::vector<LocationInfo> pathToDestination = dijkstraDriving(graph, includeNode, dest, avoidNodes, avoidSegments);
        if (pathToDestination.empty()) {
            return {};
        }
        pathToInclude.pop_back();
        fullPath.insert(fullPath.end(), pathToInclude.begin(), pathToInclude.end());
        fullPath.insert(fullPath.end(), pathToDestination.begin(), pathToDestination.end());
    } else {
        fullPath = dijkstraDriving(graph, source, dest, avoidNodes, avoidSegments);
    }

    return fullPath;
}

std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>>
GraphAlgorithms::getParkingNodes(Graph<LocationInfo> *graph, int source, const std::vector<int> &avoidNodes,
                                 const std::vector<std::pair<int, int>> &avoidSegments) {

    MutablePriorityQueue<Vertex<LocationInfo>> pq;
    for (auto &vertex : graph->getVertexSet()) {
        vertex->setDrivingDist(INF);
        vertex->setPath(nullptr);
        vertex->setQueueIndex(0);
    }
    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return {};
    }
    sourceVertex->setDrivingDist(0);
    pq.insert(sourceVertex);

    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();
        if (!avoidNodes.empty() && std::find(avoidNodes.begin(), avoidNodes.end(), currentVertex->getInfo().id) != avoidNodes.end()) {
            continue;
        }
        for (auto &edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();
            if (!avoidSegments.empty() && std::find(avoidSegments.begin(), avoidSegments.end(),
                                                    std::make_pair(currentVertex->getInfo().id, neighbor->getInfo().id)) != avoidSegments.end()) {
                continue;
            }
            if (relax(edge, true)) {
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }

    std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>> parkingNodes;
    for (auto &vertex : graph->getVertexSet()) {
        if (vertex->getInfo().parking && vertex->getDrivingDist() != INF) {
            auto path = getPath(graph, source, vertex->getInfo().id, true);
            parkingNodes.push_back(std::make_pair(vertex, path));
        }
    }
    return parkingNodes;
}

std::vector<EnvironmentalPath> GraphAlgorithms::environmentalRoute(Graph<LocationInfo> *graph, int source, int dest, double maxWalkingTime,
                                                                   const std::vector<int>                 &avoidNodes,
                                                                   const std::vector<std::pair<int, int>> &avoidSegments) {
    std::vector<EnvironmentalPath> results;

    if (source == dest) {
        EnvironmentalPath err;
        err.message = "Source node equal to destination node.";
        results.push_back(err);
        return results;
    }

    Vertex<LocationInfo> *sourceVertex = graph->findVertexById(source);
    if (sourceVertex == nullptr) {
        EnvironmentalPath err;
        err.message = "Source node not found in the graph.";
        results.push_back(err);
        return results;
    }
    Vertex<LocationInfo> *destVertex = graph->findVertexById(dest);
    if (destVertex == nullptr) {
        EnvironmentalPath err;
        err.message = "Destination node not found in the graph.";
        results.push_back(err);
        return results;
    }
    if (graph->findEdge(source, dest) != nullptr) {
        EnvironmentalPath err;
        err.message = "Source and destination cannot be adjacent.";
        results.push_back(err);
        return results;
    }
    if (sourceVertex->getInfo().parking || destVertex->getInfo().parking) {
        EnvironmentalPath err;
        err.message = "Source or destination cannot be parking nodes.";
        results.push_back(err);
        return results;
    }

    auto parkingNodes = GraphAlgorithms::getParkingNodes(graph, source, avoidNodes, avoidSegments);
    if (parkingNodes.empty()) {
        EnvironmentalPath err;
        err.message = "No suitable parking node found.";
        results.push_back(err);
        return results;
    }

    std::vector<EnvironmentalPath> validRoutes;
    std::vector<EnvironmentalPath> approximateRoutes;

    for (auto &parkingPair : parkingNodes) {
        Vertex<LocationInfo>     *parkingVertex = parkingPair.first;
        std::vector<LocationInfo> drivingPath   = parkingPair.second;

        std::vector<LocationInfo> walkingPath = GraphAlgorithms::dijkstraWalking(graph, parkingVertex->getInfo().id, dest, avoidNodes, avoidSegments);
        if (walkingPath.empty())
            continue;

        EnvironmentalPath route;
        route.parkingNode = parkingVertex;
        route.drivingPath = drivingPath;
        route.walkingPath = walkingPath;
        route.drivingTime = parkingVertex->getDrivingDist();
        route.walkingTime = graph->findVertexById(dest)->getWalkingDist();
        route.totalTime   = route.drivingTime + route.walkingTime;

        approximateRoutes.push_back(route);
        if (route.walkingTime <= maxWalkingTime)
            validRoutes.push_back(route);
    }

    if (!validRoutes.empty()) {
        std::sort(validRoutes.begin(), validRoutes.end(), [](const EnvironmentalPath &a, const EnvironmentalPath &b) {
            if (a.totalTime != b.totalTime)
                return a.totalTime < b.totalTime;
            return a.walkingTime > b.walkingTime;
        });
        results.push_back(validRoutes.front());
    } else if (!approximateRoutes.empty()) {
        std::sort(approximateRoutes.begin(), approximateRoutes.end(), [](const EnvironmentalPath &a, const EnvironmentalPath &b) {
            if (a.totalTime != b.totalTime)
                return a.totalTime < b.totalTime;
            return a.walkingTime > b.walkingTime;
        });
        results.push_back(approximateRoutes.front());
        if (approximateRoutes.size() > 1)
            results.push_back(approximateRoutes[1]);
        else {
            EnvironmentalPath dummy;
            dummy.message = "none";
            results.push_back(dummy);
        }
    } else {
        EnvironmentalPath err;
        err.message = "No valid walking route within max walking time.";
        results.push_back(err);
    }
    return results;
}
