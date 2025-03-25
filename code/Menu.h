#ifndef MENU_HPP
#define MENU_HPP

#include "GraphAlgorithms.h"
#include "data-structures/Graph.h"
#include "data-structures/LocationInfo.h"
#include <iostream>
#include <string>
#include <vector>

class Menu {
public:
  Menu();
  int buildGraph(std::string locations, std::string distances);
  void getMenuOptions();
  void processOption(int option);

  int independentRoutePlanning();
  int restrictedRoutePlanning();
  int environmentallyFriendlyRoutePlanning();
  int batchMode();

private:
  Graph<LocationInfo> graph;
};

#endif // MENU_HPP