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
  // should compute smallest distances and update graph
}

std::vector<LocationInfo> GraphAlgorithms::getPath(Graph<LocationInfo> *g,
                                                   const int &origin,
                                                   const int &dest) {
  std::vector<LocationInfo> res;

  // should compute the best path given a graph

  return res;
}