#ifndef ENVIRONMENTALPATH_H
#define ENVIRONMENTALPATH_H

#include "Graph.h"
#include "LocationInfo.h"

#include <vector>

/**
 * @struct EnvironmentalPath
 * @brief Represents a path with both driving and walking segments, along with
 * associated metadata.
 *
 * This structure is used to store information about a route that includes:
 * - A driving path with its duration.
 * - A parking node where the driving ends.
 * - A walking path with its duration.
 * - The total time for the entire route.
 * - A message providing additional details about the path.
 */
struct EnvironmentalPath {
  std::vector<LocationInfo>
      drivingPath;    /**< The sequence of locations for the driving segment. */
  double drivingTime; /**< The time required for the driving segment. */
  Vertex<LocationInfo>
      *parkingNode; /**< The parking node where the driving segment ends. */
  std::vector<LocationInfo>
      walkingPath;    /**< The sequence of locations for the walking segment. */
  double walkingTime; /**< The time required for the walking segment. */
  double totalTime;   /**< The total time for the entire route (driving +
                         walking). */
  std::string message; /**< Additional information or status about the path. */

  /**
   * @brief Default constructor for EnvironmentalPath.
   *
   * Initializes all members to their default values:
   * - Empty driving and walking paths.
   * - Driving and walking times set to 0.0.
   * - Parking node set to nullptr.
   * - Total time set to 0.0.
   * - Message set to an empty string.
   *
   * This constructor runs in constant time.
   */
  EnvironmentalPath()
      : drivingPath(), drivingTime(0.0), parkingNode(nullptr), walkingPath(),
        walkingTime(0.0), totalTime(0.0), message("") {}
};

#endif // ENVIRONMENTALPATH_H