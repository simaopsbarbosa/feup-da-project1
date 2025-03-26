#include "GraphAlgorithms.h"
#include <iostream>
#include <queue>

bool GraphAlgorithms::relax(Edge<LocationInfo> *edge) { // d[u] + w(u,v) < d[v]
  if (edge->getOrig()->getDrivingDist() + edge->getDrivingWeight() <
      edge->getDest()->getDrivingDist()) {
    edge->getDest()->setDrivingDist(edge->getOrig()->getDrivingDist() +
                                    edge->getDrivingWeight());
    edge->getDest()->setPath(edge);
    return true;
  }
  return false;
}

void GraphAlgorithms::dijkstra(
    Graph<LocationInfo> *graph, int source, bool hasRestrictions,
    const std::vector<int> &avoidNodes,
    const std::vector<std::pair<int, int>> &avoidSegments) {
  MutablePriorityQueue<Vertex<LocationInfo>> pq;

  for (auto &vertex : graph->getVertexSet()) {
    vertex->setDrivingDist(INF);
    vertex->setPath(nullptr);
  }

  auto sourceVertex = graph->findVertexById(source);
  if (!sourceVertex) {
    std::cerr << "[ERROR] Source node not found in the graph.\n";
    return;
  }

  sourceVertex->setDrivingDist(0);
  pq.insert(sourceVertex);

  while (!pq.empty()) {
    auto currentVertex = pq.extractMin();

    if (!avoidNodes.empty() &&
        std::find(avoidNodes.begin(), avoidNodes.end(),
                  currentVertex->getInfo().id) != avoidNodes.end()) {
      continue;
    }

    if (hasRestrictions && currentVertex->isVisited())
      continue;

    for (auto &edge : currentVertex->getAdj()) {
      auto neighbor = edge->getDest();

      if (!avoidSegments.empty() &&
          std::find(avoidSegments.begin(), avoidSegments.end(),
                    std::make_pair(currentVertex->getInfo().id,
                                   neighbor->getInfo().id)) !=
              avoidSegments.end()) {
        continue;
      }

      if (hasRestrictions && neighbor->isVisited())
        continue;
      if (relax(edge)) {
        if (neighbor->getQueueIndex() == 0) {
          pq.insert(neighbor);
        } else {
          pq.decreaseKey(neighbor);
        }
      }
    }
  }
}

std::vector<LocationInfo> GraphAlgorithms::getPath(Graph<LocationInfo> *g,
                                                   const int &origin,
                                                   const int &dest) {

  if (g->findVertexById(dest)->getDrivingDist() == INF) {
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
    if (currentVertex->getInfo().id != origin &&
        currentVertex->getInfo().id != dest) {
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

std::vector<LocationInfo> GraphAlgorithms::restrictedRoute(
    Graph<LocationInfo> *graph, int source, int dest,
    const std::vector<int> &avoidNodes,
    const std::vector<std::pair<int, int>> &avoidSegments, int includeNode) {
  std::vector<LocationInfo> fullPath;

  if (includeNode != -1) {
    GraphAlgorithms::dijkstra(graph, source, false, avoidNodes, avoidSegments);
    std::vector<LocationInfo> pathToInclude =
        getPath(graph, source, includeNode);

    if (pathToInclude.empty()) {
      // no path found from source to includeNode
      return {};
    }

    GraphAlgorithms::dijkstra(graph, includeNode, false, avoidNodes,
                              avoidSegments);
    std::vector<LocationInfo> pathToDestination =
        getPath(graph, includeNode, dest);

    if (pathToDestination.empty()) {
      // no path found from includeNode to destination
      return {};
    }

    pathToInclude.pop_back();
    fullPath.insert(fullPath.end(), pathToInclude.begin(), pathToInclude.end());
    fullPath.insert(fullPath.end(), pathToDestination.begin(),
                    pathToDestination.end());
  } else {
    GraphAlgorithms::dijkstra(graph, source, false, avoidNodes, avoidSegments);
    fullPath = GraphAlgorithms::getPath(graph, source, dest);
  }

  return fullPath;
}