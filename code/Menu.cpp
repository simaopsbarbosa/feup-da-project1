#include "Menu.h"

void Menu::getMenuOptions() const {
  std::cout << "--------------Route Planning--------------\n";
  std::cout << "1. Independent Route Planning\n";
  std::cout << "2. Restricted Route Planning\n";
  std::cout << "3. Environmentally-Friendly Route Planning\n";
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

int Menu::buildGraph(std::string locations, std::string distances) {
  std::ifstream file(locations);

  if (!file.is_open()) {
    std::cerr << "[ERROR] Error opening file: " << locations << std::endl;
    return 1;
  }

  std::string line;

  // read and check first line
  if (!std::getline(file, line)) {
    std::cerr << "[ERROR] Empty file: " << locations << std::endl;
    return 1;
  }

  int count = 1;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    count++;
    std::string location, code, id_str, parking_str;
    int id;
    bool parking;

    // extract values from line
    std::getline(ss, location, ',');
    std::getline(ss, id_str, ',');
    id = stoi(id_str);
    std::getline(ss, code, ',');
    std::getline(ss, parking_str, ',');
    parking = stoi(parking_str) == 1;

    const LocationInfo locInfo(id, location, code, parking);

    graph.addVertex(locInfo);
  }

  std::cout << graph.getNumVertex() << std::endl;
  file.close();
  return 0;
}

Menu::Menu() {
  if (buildGraph("../data-set/Locations.csv", "../data-set/Distances.csv") ==
      1) {
    return; // cannot read files
  }
  getMenuOptions();
}