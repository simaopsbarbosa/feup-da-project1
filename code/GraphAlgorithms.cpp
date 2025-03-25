#include "GraphAlgorithms.h"
#include <iostream>
#include <queue>

bool GraphAlgorithms::relax(Edge<LocationInfo> *edge) { // d[u] + w(u,v) < d[v]
  if (edge->getOrig()->getDrivingDist() + edge->getDrivingWeight() < edge->getDest()->getDrivingDist()) {
    edge->getDest()->setDrivingDist(edge->getOrig()->getDrivingDist() + edge->getDrivingWeight());
    edge->getDest()->setPath(edge);
    return true;
  }
  return false;
}

void GraphAlgorithms::dijkstra(Graph<LocationInfo>& graph, int source) {
    // Step 1: Initialize distances and priority queue
    MutablePriorityQueue<LocationInfo> pq;

    for (auto& vertex : graph.getVertexSet()) {
        vertex->setDrivingDist(INF); // Set all distances to infinity
        vertex->setPath(nullptr);   // No previous node initially
    }

    // Set the source vertex distance to 0 and add it to the priority queue
    auto sourceVertex = graph.findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return;
    }
    sourceVertex->setDrivingDist(0);
    pq.insert(&sourceVertex->getInfo());

    // Step 2: Process the priority queue
    while (!pq.empty()) {
        // Extract the vertex with the smallest distance
        auto current = pq.extractMin();
        auto currentVertex = graph.findVertexById(current->id);

        // Iterate over all neighbors of the current vertex
        for (auto& edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();

            // Relax the edge
            if (relax(edge)) {
                // If the neighbor is not in the queue, insert it
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(&neighbor->getInfo());
                } else {
                    // Otherwise, update its position in the queue
                    pq.decreaseKey(&neighbor->getInfo());
                }
            }
        }
    }
 }

std::vector<LocationInfo> GraphAlgorithms::getPath(Graph<LocationInfo> *g, const int &origin, const int &dest) {
  std::vector<LocationInfo> res;

  // should compute the best path given a graph

  return res;
}