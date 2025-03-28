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

std::vector<LocationInfo> GraphAlgorithms::restrictedRoute(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments, int includeNode) {
    std::vector<LocationInfo> fullPath;

    if (includeNode != -1) {
        std::vector<LocationInfo> pathToInclude = dijkstraDriving(graph, source, includeNode, avoidNodes, avoidSegments);

        if (pathToInclude.empty()) {
            // no path found from source to includeNode
            return {};
        }

        std::vector<LocationInfo> pathToDestination = dijkstraDriving(graph, includeNode, dest, avoidNodes, avoidSegments);

        if (pathToDestination.empty()) {
            // no path found from includeNode to destination
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
    // Run full Dijkstra (for driving mode) from source to compute distances to all nodes.
    MutablePriorityQueue<Vertex<LocationInfo>> pq;

    // Initialize all vertices.
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

    // Process the queue until empty.
    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();

        // Do not process vertices that are in the avoid list.
        if (!avoidNodes.empty() && std::find(avoidNodes.begin(), avoidNodes.end(), currentVertex->getInfo().id) != avoidNodes.end()) {
            continue;
        }

        for (auto &edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();

            // Skip the edge if it is in the avoid segments.
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

    // Now that every vertex has its driving distance from source,
    // iterate over all vertices and select those that are parking nodes.
    std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>> parkingNodes;
    for (auto &vertex : graph->getVertexSet()) {
        if (vertex->getInfo().parking && vertex->getDrivingDist() != INF) {
            // Reconstruct the driving path from source to this parking node.
            auto path = getPath(graph, source, vertex->getInfo().id, true);
            parkingNodes.push_back(std::make_pair(vertex, path));
        }
    }

    return parkingNodes;
}

EnvironmentalPath GraphAlgorithms::environmentalRoute(Graph<LocationInfo> *graph, int source, int dest, double maxWalkingTime,
                                                      const std::vector<int> &avoidNodes, const std::vector<std::pair<int, int>> &avoidSegments) {
    EnvironmentalPath res;

    Vertex<LocationInfo> *sourceVertex = graph->findVertexById(source);
    if (sourceVertex == nullptr) {
        res.message = "Source node not found in the graph.";
        return res;
    }
    Vertex<LocationInfo> *destVertex = graph->findVertexById(dest);
    if (destVertex == nullptr) {
        res.message = "Destination node not found in the graph.";
        return res;
    }

    if (graph->findEdge(source, dest) != nullptr) {
        res.message = "Source and destination cannot be adjacent.";
        return res;
    }

    if (sourceVertex->getInfo().parking || destVertex->getInfo().parking) {
        res.message = "Source or destination cannot be parking nodes.";
        return res;
    }

    std::vector<std::pair<Vertex<LocationInfo> *, std::vector<LocationInfo>>> parkingNodes =
            GraphAlgorithms::getParkingNodes(graph, source, avoidNodes, avoidSegments);

    if (parkingNodes.empty()) {
        res.message = "No suitable parking node found.";
        return res;
    }

    std::vector<EnvironmentalPath> validRoutes;
    for (auto &parkingPair : parkingNodes) {
        Vertex<LocationInfo>     *parkingVertex = parkingPair.first;
        std::vector<LocationInfo> drivingPath   = parkingPair.second;

        std::vector<LocationInfo> walkingPath = GraphAlgorithms::dijkstraWalking(graph, parkingVertex->getInfo().id, dest, avoidNodes, avoidSegments);

        // If no walking route found or walking time exceeds max, skip this parking node.
        if (walkingPath.empty() || destVertex->getWalkingDist() > maxWalkingTime) {
            continue;
        }

        EnvironmentalPath thisPath;
        thisPath.parkingNode = parkingVertex;
        thisPath.drivingPath = drivingPath;
        thisPath.walkingPath = walkingPath;
        thisPath.drivingTime = parkingVertex->getDrivingDist();
        thisPath.walkingTime = destVertex->getWalkingDist();
        thisPath.totalTime   = thisPath.drivingTime + thisPath.walkingTime;
        validRoutes.push_back(thisPath);
    }

    if (validRoutes.empty()) {
        res.message = "No valid walking route within max walking time.";
        return res;
    }

    std::sort(validRoutes.begin(), validRoutes.end(), [](const EnvironmentalPath &a, const EnvironmentalPath &b) {
        if (a.totalTime != b.totalTime) {
            return a.totalTime < b.totalTime;
        }
        return a.walkingTime > b.walkingTime;
    });

    res = validRoutes[0];
    return res;
}
