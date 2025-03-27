#include "GraphAlgorithms.h"

#include <iostream>
#include <queue>

bool GraphAlgorithms::relax(Edge<LocationInfo> *edge, bool isDriving) {
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

void GraphAlgorithms::dijkstra(Graph<LocationInfo> *graph, int source, bool hasRestrictions, const std::vector<int> &avoidNodes,
                               const std::vector<std::pair<int, int>> &avoidSegments, bool isDriving) {
    MutablePriorityQueue<Vertex<LocationInfo>> pq;

    for (auto &vertex : graph->getVertexSet()) {
        if (isDriving) {
            vertex->setDrivingDist(INF);
        } else {
            vertex->setWalkingDist(INF);
        }
        vertex->setPath(nullptr);
    }

    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return;
    }

    if (isDriving) {
        sourceVertex->setDrivingDist(0);
    } else {
        sourceVertex->setWalkingDist(0);
    }
    pq.insert(sourceVertex);

    while (!pq.empty()) {
        auto currentVertex = pq.extractMin();

        if (!avoidNodes.empty() && std::find(avoidNodes.begin(), avoidNodes.end(), currentVertex->getInfo().id) != avoidNodes.end()) {
            continue;
        }

        if (hasRestrictions && currentVertex->isVisited())
            continue;

        for (auto &edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();

            if (!avoidSegments.empty() && std::find(avoidSegments.begin(), avoidSegments.end(),
                                                    std::make_pair(currentVertex->getInfo().id, neighbor->getInfo().id)) != avoidSegments.end()) {
                continue;
            }

            if (hasRestrictions && neighbor->isVisited())
                continue;
            if (relax(edge, isDriving)) {
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }
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

std::vector<LocationInfo> GraphAlgorithms::restrictedRoute(Graph<LocationInfo> *graph, int source, int dest, const std::vector<int> &avoidNodes,
                                                           const std::vector<std::pair<int, int>> &avoidSegments, int includeNode) {
    std::vector<LocationInfo> fullPath;

    if (includeNode != -1) {
        GraphAlgorithms::dijkstra(graph, source, false, avoidNodes, avoidSegments);
        std::vector<LocationInfo> pathToInclude = getPath(graph, source, includeNode);

        if (pathToInclude.empty()) {
            // no path found from source to includeNode
            return {};
        }

        GraphAlgorithms::dijkstra(graph, includeNode, false, avoidNodes, avoidSegments);
        std::vector<LocationInfo> pathToDestination = getPath(graph, includeNode, dest);

        if (pathToDestination.empty()) {
            // no path found from includeNode to destination
            return {};
        }

        pathToInclude.pop_back();
        fullPath.insert(fullPath.end(), pathToInclude.begin(), pathToInclude.end());
        fullPath.insert(fullPath.end(), pathToDestination.begin(), pathToDestination.end());
    } else {
        GraphAlgorithms::dijkstra(graph, source, false, avoidNodes, avoidSegments);
        fullPath = GraphAlgorithms::getPath(graph, source, dest);
    }

    return fullPath;
}

Path GraphAlgorithms::environmentalRoute(Graph<LocationInfo> *graph, int source, int dest, int maxWalkingTime, const std::vector<int> &avoidNodes,
                                         const std::vector<std::pair<int, int>> &avoidSegments) {

    Vertex<LocationInfo> *sourceVertex = graph->findVertexById(source);
    Vertex<LocationInfo> *destVertex   = graph->findVertexById(dest);

    if (!sourceVertex || !destVertex) {
        return {};
    }

    if (sourceVertex->getInfo().parking || destVertex->getInfo().parking) {
        return {};
    }

    struct ParkingNode {
        Vertex<LocationInfo>     *node;
        std::vector<LocationInfo> drivingPath;
        double                    drivingDist;
    };

    std::vector<ParkingNode> parkingNodes;
    dijkstra(graph, source, false, avoidNodes, avoidSegments, true);

    for (auto &vertex : graph->getVertexSet()) {
        if (vertex->getInfo().parking && vertex->getDrivingDist() != INF) {
            std::vector<LocationInfo> drivingPath = getPath(graph, source, vertex->getInfo().id, true);
            if (!drivingPath.empty()) {
                parkingNodes.push_back({vertex, drivingPath, vertex->getDrivingDist()});
            }
        }
    }

    if (parkingNodes.empty()) {
        return {};
    }

    std::vector<Path> walkingPaths;
    for (auto &parkingNode : parkingNodes) {
        if (parkingNode.drivingPath.empty()) {
            continue;
        }

        int parkingNodeId = parkingNode.drivingPath.back().id;
        dijkstra(graph, parkingNodeId, false, avoidNodes, avoidSegments, false);
        std::vector<LocationInfo> thisWalkingPath = getPath(graph, parkingNodeId, dest, false);

        if (thisWalkingPath.empty()) {
            continue;
        }

        if (sourceVertex->getWalkingDist() > maxWalkingTime) {
            continue;
        }

        Path newPath;
        newPath.drivingPath = parkingNode.drivingPath;
        newPath.drivingTime = parkingNode.drivingDist;
        newPath.parkingNode = parkingNode.node;
        newPath.walkingPath = thisWalkingPath;
        newPath.walkingTime = parkingNode.node->getWalkingDist();
        newPath.totalTime   = newPath.drivingTime + newPath.walkingTime;
        walkingPaths.push_back(newPath);
    }

    if (walkingPaths.empty()) {
        return {};
    }

    std::sort(walkingPaths.begin(), walkingPaths.end(), [](const Path &a, const Path &b) {
        if (a.totalTime != b.totalTime) {
            return a.totalTime < b.totalTime;
        }
        return a.walkingTime > b.walkingTime;
    });

    return walkingPaths[0];
}
