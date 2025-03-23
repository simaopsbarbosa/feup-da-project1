#ifndef MENU_HPP
#define MENU_HPP

#include "data-structures/Graph.h"
#include <iostream>
#include <string>
#include <vector>

class Menu {
public:
  Menu();
  int buildGraph(std::string locations, std::string distances);
  void getMenuOptions() const;
  void processOption(int option) const;

private:
  Graph<LocationInfo> graph;
};

#endif // MENU_HPP