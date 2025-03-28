#ifndef MENU_HPP
#define MENU_HPP

#include "GraphAlgorithms.h"
#include "data-structures/Graph.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class Menu {
  public:
    Menu();
    int  buildGraph(std::string locations, std::string distances);
    void getMenuOptions();
    void processOption(int option);

    int                              independentRoutePlanning();
    int                              restrictedRoutePlanning();
    int                              environmentallyFriendlyRoutePlanning();
    int                              batchMode();
    std::vector<int>                 parseNodes(const std::string &input);
    std::vector<std::pair<int, int>> parseSegments(const std::string &input);

  private:
    Graph<LocationInfo> graph;
};

#endif // MENU_HPP