#ifndef LOCATION_INFO_H
#define LOCATION_INFO_H

#include <iostream>
#include <string>

struct LocationInfo {
    int         id;
    std::string location;
    std::string code;
    bool        parking;

    LocationInfo(int id, std::string location, std::string code, bool parking)
        : id(id), location(location), code(code), parking(parking) {
    }

    bool operator==(const LocationInfo &other) const {
        return id == other.id && location == other.location && code == other.code &&
               parking == other.parking;
    }

    bool operator!=(const LocationInfo &other) const {
        return !(*this == other);
    }

    bool operator<(const LocationInfo &other) const {
        return id < other.id;
    }

    friend std::ostream &operator<<(std::ostream &os, const LocationInfo &info) {
        os << "ID: " << info.id << ", Location: " << info.location << ", Code: " << info.code
           << ", Parking: " << (info.parking ? "Yes" : "No");
        return os;
    }
};

#endif // LOCATION_INFO_H