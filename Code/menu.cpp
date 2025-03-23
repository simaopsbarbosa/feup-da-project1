#include "menu.h"

void Menu::getMenuOptions() const {
  std::cout << "--------------Route Planning--------------\n";
  std::cout << "1. Independent Route Planning\n";
  std::cout << "2. Restricted Route Planning\n";
  std::cout
      << "3. Environmentally-Friendly Route Planning (driving and walking)\n";
  std::cout << "4. Batch mode\n";
  std::cout << "5. Leave\n";
  std::cout << "------------------------------------------\n";
  std::cout << "Choose an option: ";

  int option;
  std::cin >> option;
  processOption(option);
}

void Menu::processOption(int option) const {
  switch (option) {
  case 1:
    std::cout << "\nCalculating independent route...\n";
    break;

  case 2:
    std::cout << "\nCalculating restricted route...\n";
    break;

  case 3:
    std::cout << "\nCalculating environmentally-friendly route...\n";
    break;

  case 5:
    std::cout << "\nLeaving program...\n";
    return;

  default:
    std::cout << "\nInvalid option. Try again.\n";
    getMenuOptions();
  }
}

int Menu::buildGraph(std::string locations, std::string distances){
  return 0;
}

Menu::Menu() {
  buildGraph("../DataSet/Locations.csv", "../DataSet/Distances.csv");
  getMenuOptions();
}