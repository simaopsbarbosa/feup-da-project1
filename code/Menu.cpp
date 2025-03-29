#include "Menu.h"

void Menu::getMenuOptions() {
  std::cout << "--------------------ROUTE PLANNING--------------------\n";
  std::cout << "1. Independent Route Planning\n";
  std::cout << "2. Restricted Route Planning\n";
  std::cout << "3. Environmentally-Friendly Route Planning\n";
  std::cout << "4. Instructions\n";
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
    displayInstructions();
    break;

  case 5:
    std::cout << "\nLeaving program...\n";
    return;

  default:
    std::cout << "\nInvalid option. Try again.\n";
    getMenuOptions();
  }
}

void Menu::displayInstructions() {
  std::cout << "--------------------INSTRUCTIONS----------------------\n";
  std::cout << "MODES 1-3:\n";
  std::cout << "To run the program in interactive mode, select one of the "
               "first three options.\n";
  std::cout << "You will be prompted to enter the source and destination "
               "locations.\n";
  std::cout << "Some prompts, like AvoidNodes, AvoidSegments and IncludeNode "
               "may be left empty if desired.\n\n ";
  std::cout << "BATCH MODE:\n";
  std::cout << "To run the program in batch mode, provide the input file name "
               "as the third argument.\n ";
  std::cout << "You can also specify the output file name as the fourth "
               "argument.\n\n";
  getMenuOptions();
}

int Menu::independentRoutePlanning() {
  std::cout << "------------------------INPUT-------------------------\n";
  int source, dest;
  std::cout << "Source:";
  std::cin >> source;
  std::cout << "Destination:";
  std::cin >> dest;
  std::cout << "------------------------------------------------------\n";

  std::cout << "------------------------OUTPUT------------------------\n";

  std::cout << "BestDrivingRoute:";
  std::vector<LocationInfo> primaryPath =
      GraphAlgorithms::normalRoute(&graph, source, dest, {}, {});

  if (primaryPath.empty()) {
    std::cout << "none\n";
    std::cout << "AlternativeDrivingRoute:none\n";
    std::cout << "------------------------------------------------------\n";
    return 0;
  }

  for (int i = 0; i < primaryPath.size(); ++i) {
    std::cout << primaryPath[i].id;
    if (i < primaryPath.size() - 1) {
      std::cout << ",";
    }
  }
  std::cout << "(" << graph.findVertexById(dest)->getDrivingDist() << ")\n";

  std::cout << "AlternativeDrivingRoute:";

  std::vector<int> nodesToAvoid;
  for (int i = 1; i < primaryPath.size() - 1; ++i) {
    nodesToAvoid.push_back(primaryPath[i].id);
  }
  std::vector<LocationInfo> altPath =
      GraphAlgorithms::normalRoute(&graph, source, dest, nodesToAvoid, {});

  if (altPath.size() == 0 || (altPath.size() == 2 && primaryPath.size() == 2)) {
    std::cout << "none\n";
  } else {
    for (int i = 0; i < altPath.size(); ++i) {
      std::cout << altPath[i].id;
      if (i < altPath.size() - 1) {
        std::cout << ",";
      }
    }
    std::cout << "(" << graph.findVertexById(dest)->getDrivingDist() << ")\n";
  }
  std::cout << "------------------------------------------------------\n";

  return 0;
}

int Menu::restrictedRoutePlanning() {
  std::cout << "------------------------INPUT-------------------------\n";
  int source, dest, includeNode;
  std::string includeNodeInput;
  std::string avoidNodesInput, avoidSegmentsInput;
  std::cout << "Source:";
  std::cin >> source;
  std::cout << "Destination:";
  std::cin >> dest;
  std::cin.ignore();

  std::cout << "AvoidNodes: ";
  std::getline(std::cin, avoidNodesInput);
  std::vector<int> avoidNodes = parser.parseNodes(avoidNodesInput);

  std::cout << "AvoidSegments: ";
  std::getline(std::cin, avoidSegmentsInput);
  std::vector<std::pair<int, int>> avoidSegments =
      parser.parseSegments(avoidSegmentsInput);

  std::cout << "IncludeNode: ";
  std::getline(std::cin, includeNodeInput);
  includeNode = includeNodeInput.empty() ? -1 : std::stoi(includeNodeInput);
  std::cout << "------------------------------------------------------\n";

  std::cout << "------------------------OUTPUT------------------------\n";
  std::cout << "RestrictedDrivingRoute:";
  std::vector<LocationInfo> restrictedPath = GraphAlgorithms::normalRoute(
      &graph, source, dest, avoidNodes, avoidSegments, includeNode);

  if (restrictedPath.empty()) {
    std::cout << "none\n";
  } else {
    double totalWeight = 0.0;

    for (size_t i = 0; i < restrictedPath.size(); ++i) {
      std::cout << restrictedPath[i].id;
      if (i < restrictedPath.size() - 1) {
        std::cout << ",";

        auto edge =
            graph.findEdge(restrictedPath[i].id, restrictedPath[i + 1].id);
        if (edge) {
          totalWeight += edge->getDrivingWeight();
        }
      }
    }
    std::cout << "(" << totalWeight << ")\n";
  }
  std::cout << "------------------------------------------------------\n";

  return 0;
}
int Menu::environmentallyFriendlyRoutePlanning() {
  std::cout << "------------------------INPUT-------------------------\n";
  int source, dest, includeNode;
  double maxWalkingTime;
  std::string includeNodeInput;
  std::string avoidNodesInput, avoidSegmentsInput;

  std::cout << "Source:";
  std::cin >> source;
  std::cout << "Destination:";
  std::cin >> dest;
  std::cout << "MaxWalkingTime: ";
  std::cin >> maxWalkingTime;
  std::cin.ignore();

  std::cout << "AvoidNodes: ";
  std::getline(std::cin, avoidNodesInput);
  std::vector<int> avoidNodes = parser.parseNodes(avoidNodesInput);

  std::cout << "AvoidSegments: ";
  std::getline(std::cin, avoidSegmentsInput);
  std::vector<std::pair<int, int>> avoidSegments =
      parser.parseSegments(avoidSegmentsInput);

  std::cout << "IncludeNode: ";
  std::getline(std::cin, includeNodeInput);
  includeNode = includeNodeInput.empty() ? -1 : std::stoi(includeNodeInput);
  std::cout << "------------------------------------------------------\n";

  std::cout << "------------------------OUTPUT------------------------\n";
  std::vector<EnvironmentalPath> paths = GraphAlgorithms::environmentalRoute(
      &graph, source, dest, maxWalkingTime, avoidNodes, avoidSegments);

  std::cout << "Source:" << source << "\n";
  std::cout << "Destination:" << dest << "\n";
  std::cout << "MaxWalkTime:" << maxWalkingTime << "\n";

  if (paths.size() == 1) {
    const EnvironmentalPath &p = paths[0];
    std::cout << "DrivingRoute:";
    if (p.drivingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < p.drivingPath.size(); ++i) {
        std::cout << p.drivingPath[i].id;
        if (i < p.drivingPath.size() - 1)
          std::cout << ",";
      }
      std::cout
          << "("
          << graph.findVertexById(p.parkingNode->getInfo().id)->getDrivingDist()
          << ")\n";
    }
    std::cout << "ParkingNode:"
              << (p.parkingNode ? std::to_string(p.parkingNode->getInfo().id)
                                : "none")
              << "\n";
    std::cout << "WalkingRoute:";
    if (p.walkingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < p.walkingPath.size(); ++i) {
        std::cout << p.walkingPath[i].id;
        if (i < p.walkingPath.size() - 1)
          std::cout << ",";
      }
      std::cout << "(" << p.walkingTime << ")\n";
    }
    std::cout << "TotalTime:" << p.totalTime << "\n";
    if (!p.message.empty()) {
      std::cout << "Message:" << p.message << "\n";
    }
  } else if (paths.size() == 2) {
    std::cout << "DrivingRoute1:";
    if (paths[0].drivingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < paths[0].drivingPath.size(); ++i) {
        std::cout << paths[0].drivingPath[i].id;
        if (i < paths[0].drivingPath.size() - 1)
          std::cout << ",";
      }
      std::cout << "("
                << graph.findVertexById(paths[0].parkingNode->getInfo().id)
                       ->getDrivingDist()
                << ")\n";
    }
    std::cout << "ParkingNode1:"
              << (paths[0].parkingNode
                      ? std::to_string(paths[0].parkingNode->getInfo().id)
                      : "none")
              << "\n";
    std::cout << "WalkingRoute1:";
    if (paths[0].walkingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < paths[0].walkingPath.size(); ++i) {
        std::cout << paths[0].walkingPath[i].id;
        if (i < paths[0].walkingPath.size() - 1)
          std::cout << ",";
      }
      std::cout << "(" << paths[0].walkingTime << ")\n";
    }
    std::cout << "TotalTime1:" << paths[0].totalTime << "\n";

    std::cout << "DrivingRoute2:";
    if (paths[1].drivingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < paths[1].drivingPath.size(); ++i) {
        std::cout << paths[1].drivingPath[i].id;
        if (i < paths[1].drivingPath.size() - 1)
          std::cout << ",";
      }
      std::cout
          << "("
          << (paths[1].parkingNode
                  ? graph.findVertexById(paths[1].parkingNode->getInfo().id)
                        ->getDrivingDist()
                  : 0)
          << ")\n";
    }
    std::cout << "ParkingNode2:"
              << (paths[1].parkingNode
                      ? std::to_string(paths[1].parkingNode->getInfo().id)
                      : "none")
              << "\n";
    std::cout << "WalkingRoute2:";
    if (paths[1].walkingPath.empty()) {
      std::cout << "none\n";
    } else {
      for (size_t i = 0; i < paths[1].walkingPath.size(); ++i) {
        std::cout << paths[1].walkingPath[i].id;
        if (i < paths[1].walkingPath.size() - 1)
          std::cout << ",";
      }
      std::cout << "(" << paths[1].walkingTime << ")\n";
    }
    std::cout << "TotalTime2:" << paths[1].totalTime << "\n";
  }
  std::cout << "------------------------------------------------------\n";
  return 0;
}

int Menu::batchMode(std::string input, std::string output) {
  std::ifstream inputFile(input);
  if (!inputFile.is_open()) {
    std::cerr << "[ERROR] Unable to open input file: " << input << "\n";
    return 1;
  }

  if (output.empty()) {
    size_t pos = input.find_last_of("/\\");
    if (pos != std::string::npos) {
      output = input.substr(0, pos + 1) + "output.txt";
    } else {
      output = "output.txt";
    }
  }

  std::ofstream outputFile(output);
  if (!outputFile.is_open()) {
    std::cerr << "[ERROR] Unable to open or create output file: " << output
              << "\n";
    return 1;
  }
  std::string line;

  while (std::getline(inputFile, line)) {
    std::string mode = line.substr(line.find(':') + 1);

    int source, dest;
    std::getline(inputFile, line);
    source = std::stoi(line.substr(line.find(':') + 1));

    std::getline(inputFile, line);
    dest = std::stoi(line.substr(line.find(':') + 1));

    if (mode == "driving-walking") {
      double maxWalkingTime;
      std::getline(inputFile, line);
      maxWalkingTime = std::stod(line.substr(line.find(':') + 1));

      std::getline(inputFile, line);
      std::string avoidNodesStr = line.substr(line.find(':') + 1);
      std::vector<int> avoidNodes = parser.parseNodes(avoidNodesStr);

      std::getline(inputFile, line);
      std::string avoidSegmentsStr = line.substr(line.find(':') + 1);
      std::vector<std::pair<int, int>> avoidSegments =
          parser.parseSegments(avoidSegmentsStr);

      std::vector<EnvironmentalPath> paths =
          GraphAlgorithms::environmentalRoute(
              &graph, source, dest, maxWalkingTime, avoidNodes, avoidSegments);

      outputFile << "Source:" << source << "\n";
      outputFile << "Destination:" << dest << "\n";

      if (paths.size() == 1) {
        const EnvironmentalPath &p = paths[0];
        outputFile << "DrivingRoute:";
        if (p.drivingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < p.drivingPath.size(); ++i) {
            outputFile << p.drivingPath[i].id;
            if (i < p.drivingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "("
                     << graph.findVertexById(p.parkingNode->getInfo().id)
                            ->getDrivingDist()
                     << ")\n";
        }
        outputFile << "ParkingNode:"
                   << (p.parkingNode
                           ? std::to_string(p.parkingNode->getInfo().id)
                           : "none")
                   << "\n";
        outputFile << "WalkingRoute:";
        if (p.walkingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < p.walkingPath.size(); ++i) {
            outputFile << p.walkingPath[i].id;
            if (i < p.walkingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "(" << p.walkingTime << ")\n";
        }
        outputFile << "TotalTime:" << p.totalTime << "\n";
        if (!p.message.empty()) {
          outputFile << "Message:" << p.message << "\n";
        }
      } else if (paths.size() == 2) {
        outputFile << "DrivingRoute1:";
        if (paths[0].drivingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < paths[0].drivingPath.size(); ++i) {
            outputFile << paths[0].drivingPath[i].id;
            if (i < paths[0].drivingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "("
                     << graph
                            .findVertexById(paths[0].parkingNode->getInfo().id)
                            ->getDrivingDist()
                     << ")\n";
        }
        outputFile << "ParkingNode1:"
                   << (paths[0].parkingNode
                           ? std::to_string(paths[0].parkingNode->getInfo().id)
                           : "none")
                   << "\n";
        outputFile << "WalkingRoute1:";
        if (paths[0].walkingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < paths[0].walkingPath.size(); ++i) {
            outputFile << paths[0].walkingPath[i].id;
            if (i < paths[0].walkingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "(" << paths[0].walkingTime << ")\n";
        }
        outputFile << "TotalTime1:" << paths[0].totalTime << "\n";

        outputFile << "DrivingRoute2:";
        if (paths[1].drivingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < paths[1].drivingPath.size(); ++i) {
            outputFile << paths[1].drivingPath[i].id;
            if (i < paths[1].drivingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "("
                     << (paths[1].parkingNode
                             ? graph
                                   .findVertexById(
                                       paths[1].parkingNode->getInfo().id)
                                   ->getDrivingDist()
                             : 0)
                     << ")\n";
        }
        outputFile << "ParkingNode2:"
                   << (paths[1].parkingNode
                           ? std::to_string(paths[1].parkingNode->getInfo().id)
                           : "none")
                   << "\n";
        outputFile << "WalkingRoute2:";
        if (paths[1].walkingPath.empty()) {
          outputFile << "none\n";
        } else {
          for (size_t i = 0; i < paths[1].walkingPath.size(); ++i) {
            outputFile << paths[1].walkingPath[i].id;
            if (i < paths[1].walkingPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "(" << paths[1].walkingTime << ")\n";
        }
        outputFile << "TotalTime2:" << paths[1].totalTime << "\n";
      } else {
        outputFile << "DrivingRoute:\n";
        outputFile << "ParkingNode:\n";
        outputFile << "WalkingRoute:\n";
        outputFile << "TotalTime:\n";
        outputFile << "Message:No possible route with max. walking time of "
                   << maxWalkingTime << " minutes.\n";
      }
    } else if (mode == "driving") {
      std::vector<int> avoidNodes;
      std::vector<std::pair<int, int>> avoidSegments;
      int includeNode = -1;

      if (inputFile.peek() != EOF) {
        std::getline(inputFile, line);
        if (line.find("AvoidNodes:") != std::string::npos)
          avoidNodes = parser.parseNodes(line.substr(line.find(':') + 1));
      }
      if (inputFile.peek() != EOF) {
        std::getline(inputFile, line);
        if (line.find("AvoidSegments:") != std::string::npos)
          avoidSegments = parser.parseSegments(line.substr(line.find(':') + 1));
      }
      if (inputFile.peek() != EOF) {
        std::getline(inputFile, line);
        if (line.find("IncludeNode:") != std::string::npos) {
          std::string includeNodeStr = line.substr(line.find(':') + 1);
          includeNode = includeNodeStr.empty() ? -1 : std::stoi(includeNodeStr);
        }
      }

      outputFile << "Source:" << source << "\n";
      outputFile << "Destination:" << dest << "\n";

      if (avoidNodes.empty() && avoidSegments.empty() && includeNode <= 0) {
        outputFile << "BestDrivingRoute:";
        std::vector<LocationInfo> primaryPath =
            GraphAlgorithms::normalRoute(&graph, source, dest, {}, {});
        if (primaryPath.empty()) {
          outputFile << "none\n";
          outputFile << "AlternativeDrivingRoute:none\n";
        } else {
          for (size_t i = 0; i < primaryPath.size(); ++i) {
            outputFile << primaryPath[i].id;
            if (i < primaryPath.size() - 1)
              outputFile << ",";
          }
          outputFile << "(" << graph.findVertexById(dest)->getDrivingDist()
                     << ")\n";
          outputFile << "AlternativeDrivingRoute:";
          std::vector<int> nodesToAvoid;
          for (size_t i = 1; i < primaryPath.size() - 1; ++i) {
            nodesToAvoid.push_back(primaryPath[i].id);
          }
          std::vector<LocationInfo> altPath = GraphAlgorithms::normalRoute(
              &graph, source, dest, nodesToAvoid, {});
          if (altPath.empty() ||
              (altPath.size() == 2 && primaryPath.size() == 2)) {
            outputFile << "none\n";
          } else {
            for (size_t i = 0; i < altPath.size(); ++i) {
              outputFile << altPath[i].id;
              if (i < altPath.size() - 1)
                outputFile << ",";
            }
            outputFile << "(" << graph.findVertexById(dest)->getDrivingDist()
                       << ")\n";
          }
        }
      } else {
        outputFile << "RestrictedDrivingRoute:";
        std::vector<LocationInfo> restrictedPath = GraphAlgorithms::normalRoute(
            &graph, source, dest, avoidNodes, avoidSegments, includeNode);
        if (restrictedPath.empty()) {
          outputFile << "none\n";
        } else {
          double totalWeight = 0.0;
          for (size_t i = 0; i < restrictedPath.size(); ++i) {
            outputFile << restrictedPath[i].id;
            if (i < restrictedPath.size() - 1) {
              outputFile << ",";
              auto edge = graph.findEdge(restrictedPath[i].id,
                                         restrictedPath[i + 1].id);
              if (edge)
                totalWeight += edge->getDrivingWeight();
            }
          }
          outputFile << "(" << totalWeight << ")\n";
        }
      }
    }
  }

  inputFile.close();
  outputFile.close();
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

    if (!graph.addBidirectionalEdge(
            graph.findVertexByCode(location1)->getInfo(),
            graph.findVertexByCode(location2)->getInfo(), dd, dw)) {
      std::cerr << "[ERROR] Cannot build bidirectional edge: from " << location1
                << " to " << location2 << ", with dd " << dd << " and dw " << dw
                << std::endl;
    } else {
      edgeCounter++;
    }
  }

  distancesFile.close();

  std::cout << "Success, " << edgeCounter << " distances detected.\n";

  return 0;
}

Menu::Menu(std::string locations, std::string distances, std::string inputFile,
           std::string outputFile)
    : parser() {
  std::cout << "\n";
  if (buildGraph(locations, distances) == 1) {
    return; // cannot read files
  }
  std::cout << "Graph built successfully.\n\n";

  if (!inputFile.empty()) {
    std::cout << "Batch mode activated.\n";
    batchMode(inputFile, outputFile);
  } else {
    getMenuOptions();
  }
}