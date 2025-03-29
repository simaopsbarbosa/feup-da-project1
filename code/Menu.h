#ifndef MENU_HPP
#define MENU_HPP

#include "GraphAlgorithms.h"
#include "InputParser.h"
#include "data-structures/Graph.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @class Menu
 * @brief Handles the user interface and interaction for route planning.
 */
class Menu {
public:
  /**
   * @brief Constructs a Menu object and initializes the graph.
   * @param locations Path to the locations file.
   * @param distances Path to the distances file.
   * @param inputFile Path to the batch mode input file (optional).
   * @param outputFile Path to the batch mode output file (optional).
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   */
  Menu(std::string locations, std::string distances, std::string inputFile = "",
       std::string outputFile = "");

  /**
   * @brief Displays the menu options to the user.
   *
   * Complexity: O(1).
   */
  void getMenuOptions();

  /**
   * @brief Processes the selected menu option.
   * @param option The selected menu option.
   *
   * Complexity: O(1).
   */
  void processOption(int option);

  /**
   * @brief Handles independent route planning.
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   * @return Status code (0 for success).
   */
  int independentRoutePlanning();

  /**
   * @brief Handles restricted route planning with constraints.
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   * @return Status code (0 for success).
   */
  int restrictedRoutePlanning();

  /**
   * @brief Handles environmentally-friendly route planning.
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   * @return Status code (0 for success).
   */
  int environmentallyFriendlyRoutePlanning();

  /**
   * @brief Executes batch mode operations based on input and output files.
   * @param inputFile Path to the batch mode input file.
   * @param outputFile Path to the batch mode output file (optional).
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   * @return Status code (0 for success).
   */
  int batchMode(std::string inputFile, std::string outputFile = "");

  /**
   * @brief Builds the graph from the provided locations and distances files.
   * @param locations Path to the locations file.
   * @param distances Path to the distances file.
   *
   * Complexity: O(V + E), where V is the number of vertices and E is the number
   * of edges.
   * @return Status code (0 for success, 1 for failure).
   */
  int buildGraph(std::string locations, std::string distances);

  /**
   * @brief Displays instructions for using the program.
   *
   * Complexity: O(1).
   */
  void displayInstructions();

private:
  Graph<LocationInfo>
      graph;          ///< The graph representing the locations and distances.
  InputParser parser; ///< Utility for parsing input data.
};

#endif // MENU_HPP