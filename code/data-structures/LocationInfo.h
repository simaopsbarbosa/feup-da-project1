#ifndef LOCATION_INFO_H
#define LOCATION_INFO_H

#include <iostream>
#include <string>

/**
 * @struct LocationInfo
 * @brief Represents information about a location.
 */
struct LocationInfo {
  int id;               /**< Unique identifier for the location. */
  std::string location; /**< Name or description of the location. */
  std::string code;     /**< Code associated with the location. */
  bool parking; /**< Indicates whether parking is available at the location. */

  /**
   * @brief Constructs a LocationInfo object.
   * @param id The unique identifier for the location.
   * @param location The name or description of the location.
   * @param code The code associated with the location.
   * @param parking Whether parking is available at the location.
   */
  LocationInfo(int id, std::string location, std::string code, bool parking)
      : id(id), location(location), code(code), parking(parking) {}

  /**
   * @brief Compares two LocationInfo objects for equality.
   * @param other The other LocationInfo object to compare.
   * @return True if all attributes are equal, false otherwise.
   * @details This operation has constant time complexity.
   */
  bool operator==(const LocationInfo &other) const {
    return id == other.id && location == other.location && code == other.code &&
           parking == other.parking;
  }

  /**
   * @brief Compares two LocationInfo objects for inequality.
   * @param other The other LocationInfo object to compare.
   * @return True if any attribute is different, false otherwise.
   * @details This operation has constant time complexity.
   */
  bool operator!=(const LocationInfo &other) const { return !(*this == other); }

  /**
   * @brief Compares two LocationInfo objects to determine ordering.
   * @param other The other LocationInfo object to compare.
   * @return True if this object's ID is less than the other's ID, false
   * otherwise.
   * @details This operation has constant time complexity.
   */
  bool operator<(const LocationInfo &other) const { return id < other.id; }

  /**
   * @brief Outputs the LocationInfo object to a stream.
   * @param os The output stream.
   * @param info The LocationInfo object to output.
   * @return The output stream.
   * @details This operation has constant time complexity.
   */
  friend std::ostream &operator<<(std::ostream &os, const LocationInfo &info) {
    os << "ID: " << info.id << ", Location: " << info.location
       << ", Code: " << info.code
       << ", Parking: " << (info.parking ? "Yes" : "No");
    return os;
  }
};

#endif // LOCATION_INFO_H