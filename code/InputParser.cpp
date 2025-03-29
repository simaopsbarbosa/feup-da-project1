#include "InputParser.h"
#include <sstream>

std::vector<int> InputParser::parseNodes(const std::string &input) {
  std::vector<int> nodes;
  std::istringstream stream(input);
  int node;
  while (stream >> node) {
    nodes.push_back(node);
  }
  return nodes;
}

std::vector<std::pair<int, int>>
InputParser::parseSegments(const std::string &input) {
  std::vector<std::pair<int, int>> segments;
  if (!input.empty()) {
    std::stringstream ss(input);
    std::string segment;
    while (std::getline(ss, segment, ')')) {
      segment.erase(std::remove(segment.begin(), segment.end(), ' '),
                    segment.end());
      if (segment.empty() || segment == ",")
        continue;
      size_t startPos = segment.find('(');
      if (startPos != std::string::npos) {
        std::string seg = segment.substr(startPos + 1);
        size_t separatorPos = seg.find(',');
        if (separatorPos != std::string::npos) {
          int node1 = std::stoi(seg.substr(0, separatorPos));
          int node2 = std::stoi(seg.substr(separatorPos + 1));
          segments.emplace_back(node1, node2);
        } else {
          continue;
        }
      }
    }
  }
  return segments;
}
