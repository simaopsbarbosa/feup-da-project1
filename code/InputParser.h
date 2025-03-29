#ifndef INPUTPARSER_HPP
#define INPUTPARSER_HPP

#include <string>
#include <utility>
#include <vector>

class InputParser {
public:
  std::vector<int> parseNodes(const std::string &input);
  std::vector<std::pair<int, int>> parseSegments(const std::string &input);
};

#endif // INPUTPARSER_HPP
