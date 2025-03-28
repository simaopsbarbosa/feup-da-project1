#ifndef ENVIRONMENTALPATH_H
#define ENVIRONMENTALPATH_H

#include "Graph.h"
#include "LocationInfo.h"

#include <vector>

struct EnvironmentalPath {
    std::vector<LocationInfo> drivingPath;
    double                    drivingTime;
    Vertex<LocationInfo>     *parkingNode;
    std::vector<LocationInfo> walkingPath;
    double                    walkingTime;
    double                    totalTime;
    std::string               message;

    EnvironmentalPath() : drivingPath(), drivingTime(0.0), parkingNode(nullptr), walkingPath(), walkingTime(0.0), totalTime(0.0), message("") {
    }
};

#endif // ENVIRONMENTALPATH_H