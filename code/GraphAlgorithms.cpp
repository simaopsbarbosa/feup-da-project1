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

std::vector<LocationInfo> GraphAlgorithms::dijkstraDriving(Graph<LocationInfo> *graph, int source, const int &dest,
                                                           const std::vector<int>                 &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;
    std::vector<LocationInfo>                  path;

    for (auto &vertex : graph->getVertexSet()) {
        vertex->setDrivingDist(INF);
        vertex->setPath(nullptr);
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

std::vector<LocationInfo> GraphAlgorithms::dijkstraWalking(Graph<LocationInfo> *graph, int source, const int &dest,
                                                           const std::vector<int>                 &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;
    std::vector<LocationInfo>                  path;

    for (auto &vertex : graph->getVertexSet()) {
        vertex->setWalkingDist(INF);
        vertex->setPath(nullptr);
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

std::vector<Vertex<LocationInfo> *> GraphAlgorithms::getParkingNodes(Graph<LocationInfo> *graph, int source, const std::vector<int> &avoidNodes,
                                                                     const std::vector<std::pair<int, int>> &avoidSegments) {
    std::vector<Vertex<LocationInfo> *> parkingNodes;

    dijkstraDriving(graph, source, -1, avoidNodes, avoidSegments);

    for (auto &vertex : graph->getVertexSet()) {
        if (vertex->getInfo().parking && vertex->getDrivingDist() != INF) {
            parkingNodes.push_back(vertex);
        }
    }

    return parkingNodes;
}
