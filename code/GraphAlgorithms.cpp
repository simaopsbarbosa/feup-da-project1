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

void GraphAlgorithms::dijkstra(Graph<LocationInfo> *graph, int source) {
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
    for (auto &edge : currentVertex->getAdj()) {
      auto neighbor = edge->getDest();
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
  std::vector<LocationInfo> res;

  auto currentVertex = g->findVertexById(dest);
  if (!currentVertex) {
    std::cerr << "[ERROR] Destination node not found in the graph.\n";
    return res;
  }

  while (currentVertex != nullptr && currentVertex->getPath() != nullptr) {
    res.push_back(currentVertex->getInfo());
    currentVertex = currentVertex->getPath()->getOrig();
  }

  if (currentVertex != nullptr && currentVertex->getInfo().id == origin) {
    res.push_back(currentVertex->getInfo());
  } else {
    std::cerr << "[ERROR] No path found from origin to destination.\n";
    return {};
  }

  std::reverse(res.begin(), res.end());
  return res;
}