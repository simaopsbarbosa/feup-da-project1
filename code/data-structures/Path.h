#ifndef PATH_H
#define PATH_H

#include <vector>
#include "LocationInfo.h"
#include "Graph.h"

struct Path {
    std::vector<LocationInfo> drivingPath;
    double                    drivingTime;
    Vertex<LocationInfo>     *parkingNode;
    std::vector<LocationInfo> walkingPath;
    double                    walkingTime;
    double                    totalTime;

    Path()
        : drivingPath(),
          drivingTime(0.0),
          parkingNode(nullptr),
          walkingPath(),
          walkingTime(0.0),
          totalTime(0.0) {}
};

#endif // PATH_H