#ifndef INPUTPARSER_HPP
#define INPUTPARSER_HPP

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

/**
 * @class InputParser
 * @brief A utility class for parsing input strings into nodes and segments.
 */
class InputParser {
public:
  /**
   * @brief Parses a string of integers into a vector of nodes.
   *
   * The input string should contain integers separated by spaces.
   * The algorithm iterates through the input string once, making it's
   * complexity O(n), where n is the number of integers.
   *
   * @param input The input string containing node data.
   * @return A vector of integers representing the nodes.
   */
  std::vector<int> parseNodes(const std::string &input);

  /**
   * @brief Parses a string of segments into a vector of pairs of integers.
   *
   * The input string should contain segments in the format "(x,y)", separated
   * by spaces or other delimiters. The algorithm processes each segment in the
   * input string, making its complexity O(m), where m is the number of segments.
   *
   * @param input The input string containing segment data.
   * @return A vector of pairs of integers representing the segments.
   */
  std::vector<std::pair<int, int>> parseSegments(const std::string &input);
};

#endif // INPUTPARSER_HPP
