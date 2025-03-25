#include "Menu.h"

void Menu::getMenuOptions() {
  std::cout << "\n--------------------ROUTE PLANNING--------------------\n";
  std::cout << "1. Independent Route Planning\n";
  std::cout << "2. Restricted Route Planning\n";
  std::cout << "3. Environmentally-Friendly Route Planning\n";
  std::cout << "4. Batch mode\n";
  std::cout << "5. Leave\n";
  std::cout << "------------------------------------------------------\n";
  std::cout << "Choose an option: ";
  
  int option;
  std::cin >> option;
  std::cout << "\n";
  
  processOption(option);
}

void Menu::processOption(int option) {
  switch (option) {
    case 1:
    independentRoutePlanning();
    break;
    
    case 2:
    restrictedRoutePlanning();
    break;
    
    case 3:
    environmentallyFriendlyRoutePlanning();
    break;
    
    case 4:
    batchMode();
    break;
    
    case 5:
    std::cout << "\nLeaving program...\n";
    return;
    
    default:
    std::cout << "\nInvalid option. Try again.\n";
    getMenuOptions();
  }
}

int Menu::independentRoutePlanning() {
  
  // get input
  int source, dest;
  std::cout << "------------------------INPUT-------------------------\n";
  std::cout << "Source node's ID: ";
  std::cin >> source;
  std::cout << "Destination node's ID: ";
  std::cin >> dest;
  std::cout << "------------------------------------------------------\n";
  
  // compute output
  GraphAlgorithms::dijkstra(&graph, source);
  std::vector<LocationInfo> path =
  GraphAlgorithms::getPath(&graph, source, dest);
  
  // show output
  std::cout << "------------------------OUTPUT------------------------\n";
  for (int i = 0; i < path.size(); ++i) {
    std::cout << path[i].id;
    if (i < path.size() - 1) {
      std::cout << ",";
    }
  }
  std::cout << " ("<< graph.findVertexById(dest)->getDrivingDist() << ")\n";
  std::cout << "------------------------------------------------------\n";
  
  return 0;
}

int Menu::restrictedRoutePlanning() {
  std::cout << "\nCalculating restricted route...\n";
  return 0;
}
int Menu::environmentallyFriendlyRoutePlanning() {
  std::cout << "\nCalculating environmentally-friendly route...\n";
  return 0;
}
int Menu::batchMode() {
  std::cout << "\nEntering batch mode...\n";
  return 0;
}

int Menu::buildGraph(std::string locations, std::string distances) {

  // ####################### PARSE LOCATIONS #######################

  std::ifstream locationsFile(locations);

  if (!locationsFile.is_open()) {
    std::cerr << "[ERROR] Error opening file: " << locations << std::endl;
    return 1;
  }

  std::string locationLine;

  // read and check first line
  if (!std::getline(locationsFile, locationLine)) {
    std::cerr << "[ERROR] Empty file: " << locations << std::endl;
    return 1;
  }

  while (std::getline(locationsFile, locationLine)) {
    std::stringstream ss(locationLine);
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

    const LocationInfo locInfo{id, location, code, parking};

    if (!graph.addVertex(locInfo)) {
      std::cerr << "[ERROR] Cannot build location: " << locInfo << std::endl;
    }
  }

  locationsFile.close();

  std::cout << "Success, " << graph.getNumVertex() << " locations detected.\n";

  // ####################### PARSE DISTANCES #######################

  std::ifstream distancesFile(distances);

  if (!distancesFile.is_open()) {
    std::cerr << "[ERROR] Error opening file: " << distances << std::endl;
    return 1;
  }

  std::string distancesLine;

  // read and check first line
  if (!std::getline(distancesFile, distancesLine)) {
    std::cerr << "[ERROR] Empty file: " << distances << std::endl;
    return 1;
  }

  int edgeCounter = 0;
  while (std::getline(distancesFile, distancesLine)) {
    std::stringstream ss(distancesLine);
    std::string location1, location2, dd_str, dw_str;
    double dd, dw;

    // extract values from line
    std::getline(ss, location1, ',');
    std::getline(ss, location2, ',');
    std::getline(ss, dd_str, ',');
    if (dd_str == "X") {
      dd = INF;
    } else {
      dd = stoi(dd_str);
    }
    std::getline(ss, dw_str, ',');
    if (dw_str == "X") {
      dw = INF;
    } else {
      dw = stoi(dw_str);
    }

    if (!graph.addEdge(graph.findVertexByCode(location1)->getInfo(),
                       graph.findVertexByCode(location2)->getInfo(), dd, dw)) {
      std::cerr << "[ERROR] Cannot build edge: from " << location1 << " to "
                << location2 << ", with dd " << dd << " and dw " << dw
                << std::endl;
    }

    else {
      edgeCounter++;
    }

    if (!graph.addEdge(graph.findVertexByCode(location2)->getInfo(),
                       graph.findVertexByCode(location1)->getInfo(), dd, dw)) {
      std::cerr << "[ERROR] Cannot build edge: from " << location2 << " to "
                << location1 << ", with dd " << dd << " and dw " << dw
                << std::endl;
    }

    else {
      edgeCounter++;
    }
  }

  distancesFile.close();

  std::cout << "Success, " << edgeCounter << " distances detected.\n";

  return 0;
}

Menu::Menu() {
  std::cout << "\n";
  if (buildGraph("../data-set/Locations2.csv", "../data-set/Distances2.csv") ==
      1) {
    return; // cannot read files
  }
  std::cout << "Graph built successfully.\n";
  getMenuOptions();
}