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

void GraphAlgorithms::dijkstra(Graph<LocationInfo>* graph, int source) {
    // Step 1: Initialize distances and priority queue
    MutablePriorityQueue<Vertex<LocationInfo>> pq;

    for (auto& vertex : graph->getVertexSet()) {
        vertex->setDrivingDist(INF); // Set all distances to infinity
        vertex->setPath(nullptr);   // No previous node initially
    }

    // Set the source vertex distance to 0 and add it to the priority queue
    auto sourceVertex = graph->findVertexById(source);
    if (!sourceVertex) {
        std::cerr << "[ERROR] Source node not found in the graph.\n";
        return;
    }
    sourceVertex->setDrivingDist(0);
    pq.insert(sourceVertex);

    // Step 2: Process the priority queue
    while (!pq.empty()) {
        // Extract the vertex with the smallest distance
        auto currentVertex = pq.extractMin();

        // Iterate over all neighbors of the current vertex
        for (auto& edge : currentVertex->getAdj()) {
            auto neighbor = edge->getDest();
        
            // Relax the edge
            if (relax(edge)) {
                // If the neighbor is not in the queue, insert it
                if (neighbor->getQueueIndex() == 0) {
                    pq.insert(neighbor);
                } else {
                    // Otherwise, update its position in the queue
                    pq.decreaseKey(neighbor);
                }
            }
        }
    }
 }

 std::vector<LocationInfo> GraphAlgorithms::getPath(Graph<LocationInfo> *g, const int &origin, const int &dest) {
    std::vector<LocationInfo> res;

    // Find the destination vertex
    auto currentVertex = g->findVertexById(dest);
    if (!currentVertex) {
        std::cerr << "[ERROR] Destination node not found in the graph.\n";
        return res; // Return an empty vector if the destination is not found
    }

    // Reconstruct the path from destination to origin
    while (currentVertex != nullptr && currentVertex->getPath() != nullptr) {
        res.push_back(currentVertex->getInfo()); // Add the current vertex to the path
        currentVertex = currentVertex->getPath()->getOrig(); // Move to the predecessor
    }

    // Add the origin vertex to the path
    if (currentVertex != nullptr && currentVertex->getInfo().id == origin) {
        res.push_back(currentVertex->getInfo());
    } else {
        std::cerr << "[ERROR] No path found from origin to destination.\n";
        return {}; // Return an empty vector if no path exists
    }

    // Reverse the path to get it in the correct order (from origin to destination)
    std::reverse(res.begin(), res.end());
    return res;
}