#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"

namespace Environment {
        bool is_adjacent(Graph<LocationInfo>* graph, const int &origin, const int &dest);
        bool is_parking_node(Graph<LocationInfo>* graph, const int &node);
        std::vector<Vertex<LocationInfo>*> get_parking_nodes(Graph<LocationInfo>* graph);
        void dijkstra(Graph<LocationInfo>* graph, int source);
        bool relax(Edge<LocationInfo> *edge, bool isWalking);
        std::vector<LocationInfo> getPath(Graph<LocationInfo> *graph, const int &origin, const int &dest, double maxWalkingTime);
};

#endif //ENVIRONMENT_H
