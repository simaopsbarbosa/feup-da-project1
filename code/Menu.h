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

class Menu {
public:
  Menu(std::string locations, std::string distances, std::string inputFile = "",
       std::string outputFile = "");
  int buildGraph(std::string locations, std::string distances);
  void getMenuOptions();
  void displayInstructions();
  void processOption(int option);
  int independentRoutePlanning();
  int restrictedRoutePlanning();
  int environmentallyFriendlyRoutePlanning();
  int batchMode(std::string inputFile, std::string outputFile = "");

private:
  Graph<LocationInfo> graph;
  InputParser parser;
};

#endif // MENU_HPP