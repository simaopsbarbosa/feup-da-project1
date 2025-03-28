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
    std::cout << "------------------------INPUT-------------------------\n";
    int source, dest;
    std::cout << "Source:";
    std::cin >> source;
    std::cout << "Destination:";
    std::cin >> dest;
    std::cout << "------------------------------------------------------\n";

    std::cout << "------------------------OUTPUT------------------------\n";

    std::cout << "BestDrivingRoute:";
    std::vector<LocationInfo> primaryPath = GraphAlgorithms::normalRoute(&graph, source, dest, {}, {});

    if (primaryPath.empty()) {
        std::cout << "none\n";
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
    std::vector<LocationInfo> altPath = GraphAlgorithms::normalRoute(&graph, source, dest, nodesToAvoid, {});

    if (altPath.size() == 0 || (altPath.size() == 2 && primaryPath.size() == 2)) {
        // altPath can be 2 , as long as primaryPath isnt 2 too (they would be the
        // same)
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
    int         source, dest, includeNode;
    std::string includeNodeInput;
    std::string avoidNodesInput, avoidSegmentsInput;
    std::cout << "Source:";
    std::cin >> source;
    std::cout << "Destination:";
    std::cin >> dest;
    std::cin.ignore();

    std::cout << "AvoidNodes: ";
    std::getline(std::cin, avoidNodesInput);
    std::vector<int> avoidNodes = parseNodes(avoidNodesInput);

    std::cout << "AvoidSegments: ";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseSegments(avoidSegmentsInput);

    std::cout << "IncludeNode: ";
    std::getline(std::cin, includeNodeInput);
    includeNode = includeNodeInput.empty() ? -1 : std::stoi(includeNodeInput);
    std::cout << "------------------------------------------------------\n";

    std::cout << "------------------------OUTPUT------------------------\n";
    std::cout << "RestrictedDrivingRoute:";
    std::vector<LocationInfo> restrictedPath = GraphAlgorithms::normalRoute(&graph, source, dest, avoidNodes, avoidSegments, includeNode);

    if (restrictedPath.empty()) {
        std::cout << "none\n";
    } else {
        double totalWeight = 0.0;

        for (size_t i = 0; i < restrictedPath.size(); ++i) {
            std::cout << restrictedPath[i].id;
            if (i < restrictedPath.size() - 1) {
                std::cout << ",";

                auto edge = graph.findEdge(restrictedPath[i].id, restrictedPath[i + 1].id);
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
    int         source, dest, includeNode;
    double      maxWalkingTime;
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
    std::vector<int> avoidNodes = parseNodes(avoidNodesInput);

    std::cout << "AvoidSegments: ";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseSegments(avoidSegmentsInput);

    std::cout << "IncludeNode: ";
    std::getline(std::cin, includeNodeInput);
    includeNode = includeNodeInput.empty() ? -1 : std::stoi(includeNodeInput);
    std::cout << "------------------------------------------------------\n";

    std::cout << "------------------------OUTPUT------------------------\n";
    std::vector<EnvironmentalPath> paths = GraphAlgorithms::environmentalRoute(&graph, source, dest, maxWalkingTime, avoidNodes, avoidSegments);

    std::cout << "Source:" << source << "\n";
    std::cout << "Destination:" << dest << "\n";
    std::cout << "MaxWalkTime:" << maxWalkingTime << "\n";

    // Display a single result if available (T3.1).
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
            std::cout << "(" << graph.findVertexById(p.parkingNode->getInfo().id)->getDrivingDist() << ")\n";
        }
        std::cout << "ParkingNode:" << (p.parkingNode ? std::to_string(p.parkingNode->getInfo().id) : "none") << "\n";
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
        // If two alternatives are returned (approximate solution), show both.
        std::cout << "DrivingRoute1:";
        if (paths[0].drivingPath.empty()) {
            std::cout << "none\n";
        } else {
            for (size_t i = 0; i < paths[0].drivingPath.size(); ++i) {
                std::cout << paths[0].drivingPath[i].id;
                if (i < paths[0].drivingPath.size() - 1)
                    std::cout << ",";
            }
            std::cout << "(" << graph.findVertexById(paths[0].parkingNode->getInfo().id)->getDrivingDist() << ")\n";
        }
        std::cout << "ParkingNode1:" << (paths[0].parkingNode ? std::to_string(paths[0].parkingNode->getInfo().id) : "none") << "\n";
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
            std::cout << "(" << (paths[1].parkingNode ? graph.findVertexById(paths[1].parkingNode->getInfo().id)->getDrivingDist() : 0) << ")\n";
        }
        std::cout << "ParkingNode2:" << (paths[1].parkingNode ? std::to_string(paths[1].parkingNode->getInfo().id) : "none") << "\n";
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

int Menu::batchMode() {
    std::ifstream inputFile("../data-set/input.txt");
    std::ofstream outputFile("../data-set/output.txt");
	std::string line;

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "[ERROR] Unable to open batch input or output file.\n";
        return 1;
    }

    while (std::getline(inputFile, line)) {
        std::string mode = line.substr(6);
        int source, dest;
        std::getline(inputFile, line);
        source = std::stoi(line.substr(8));
        std::getline(inputFile, line);
        dest = std::stoi(line.substr(12));

        if (mode == "driving-walking") {
			double maxWalkingTime;
            std::getline(inputFile, line);
            maxWalkingTime = std::stod(line.substr(15));

            std::vector<int> avoidNodes;
            std::getline(inputFile, line);
            avoidNodes = parseNodes(line.substr(11));

            std::vector<std::pair<int, int>> avoidSegments;
            std::getline(inputFile, line);
            avoidSegments = parseSegments(line.substr(15));

            EnvironmentalPath path = GraphAlgorithms::environmentalRoute(&graph, source, dest, maxWalkingTime, avoidNodes, avoidSegments);

            outputFile << "Source:" << source << "\n";
            outputFile << "Destination:" << dest << "\n";
            outputFile << "DrivingRoute:";
            if (path.drivingPath.empty()) {
                outputFile << "none\n";
            } else {
                for (int i = 0; i < path.drivingPath.size(); ++i) {
                    outputFile << path.drivingPath[i].id;
                    if (i < path.drivingPath.size() - 1) {
                        outputFile << ",";
                    }
                }
                outputFile << "(" << path.drivingTime << ")\n";
            }
            outputFile << "ParkingNode:";
            if (path.parkingNode == nullptr) {
                outputFile << "none\n";
            } else {
            	outputFile << path.parkingNode->getInfo().id << "\n";
            }
			outputFile << "WalkingRoute:";
            if (path.walkingPath.empty()) {
                outputFile << "none\n";
            } else {
                for (int i = 0; i < path.walkingPath.size(); ++i) {
                    outputFile << path.walkingPath[i].id;
                    if (i < path.walkingPath.size() - 1) {
                        outputFile << ",";
                    }
                }
                outputFile << "(" << path.walkingTime << ")\n";
            }
			outputFile << "TotalTime:" << path.totalTime << "\n";

        } else if (mode == "driving") {
            std::vector<int> avoidNodes;
            std::getline(inputFile, line);
            avoidNodes = parseNodes(line.substr(11));

            std::vector<std::pair<int, int>> avoidSegments;
            std::getline(inputFile, line);
            avoidSegments = parseSegments(line.substr(15));

            int includeNode;
        	std::getline(inputFile, line);
        	includeNode = std::stoi(line.substr(13));

			if (avoidNodes.empty() && avoidSegments.empty() && includeNode <= 0) {
                std::vector<LocationInfo> path;
                path = GraphAlgorithms::dijkstraDriving(&graph, source, dest, {}, {});
                outputFile << "Source:" << source << "\n";
                outputFile << "Destination:" << dest << "\n";
                if (path.empty()) {
                    outputFile << "none\n";
                } else {
                    outputFile << "BestDrivingRoute:";
                    for (int i = 0; i < path.size(); ++i) {
                        outputFile << path[i].id;
                        if (i < path.size() - 1) {
                            outputFile << ",";
                        }
                    }
                    outputFile << "(" << path.size() << ")\n";

                    outputFile << "AlternativeDrivingRoute:";
                    std::vector<int> nodesToAvoid;

                    for (int i = 0; i < path.size() - 1; ++i) {
                        nodesToAvoid.push_back(path[i].id);
                    }

                    std::vector<LocationInfo> altPath;
                    altPath = GraphAlgorithms::dijkstraDriving(&graph, source, dest, nodesToAvoid, {});

                    if (altPath.size() == 0 || (altPath.size() == 2 && path.size() == 2)) {
                        outputFile << "none\n";
                    } else {
                        for (int i = 0; i < altPath.size(); ++i) {
                            outputFile << altPath[i].id;
                            if (i < altPath.size() - 1) {
                                outputFile << ",";
                            }
                        }
                        outputFile << "(" << path.size() << ")\n";
                    }
                }
			} else {
                outputFile << "RestrictedDrivingRoute:";
                std::vector<LocationInfo> restrictedPath;
                restrictedPath = GraphAlgorithms::restrictedRoute(&graph, source, dest, avoidNodes, avoidSegments, includeNode);

                if (restrictedPath.empty()) {
                    outputFile << "none\n";
                } else {
                    double totalWeight = 0.0;
                    for (int i = 0; i < restrictedPath.size(); ++i) {
                        outputFile << restrictedPath[i].id;
                        if (i < restrictedPath.size() - 1) {
                            outputFile << ",";
                            auto edge = graph.findEdge(restrictedPath[i].id, restrictedPath[i + 1].id);
                            if (edge) {
                                totalWeight += edge->getDrivingWeight();
                            }
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

std::vector<int> Menu::parseNodes(const std::string &input) {
    std::vector<int> nodes;
    if (!input.empty()) {
        std::stringstream ss(input);
        std::string       node;
        while (std::getline(ss, node, ',')) {
            nodes.push_back(std::stoi(node));
        }
    }
    return nodes;
}

std::vector<std::pair<int, int>> Menu::parseSegments(const std::string &input) {
    std::vector<std::pair<int, int>> segments;
    if (!input.empty()) {
        std::stringstream ss(input);
        std::string       segment;

        while (std::getline(ss, segment, ')')) {
            segment.erase(std::remove(segment.begin(), segment.end(), ' '), segment.end());

            size_t startPos = segment.find('(');
            if (startPos != std::string::npos) {
                segment = segment.substr(startPos + 1);

                size_t separatorPos = segment.find(',');
                if (separatorPos != std::string::npos) {
                    int node1 = std::stoi(segment.substr(0, separatorPos));
                    int node2 = std::stoi(segment.substr(separatorPos + 1));
                    segments.emplace_back(node1, node2);
                } else {
                    std::cerr << "[ERROR] Invalid segment format: " << segment << "\n";
                }
            } else {
                std::cerr << "[ERROR] Invalid segment format: " << segment << "\n";
            }
        }
    }
    return segments;
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
        std::string       location, code, id_str, parking_str;
        int               id;
        bool              parking;

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
        std::string       location1, location2, dd_str, dw_str;
        double            dd, dw;

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

        if (!graph.addEdge(graph.findVertexByCode(location1)->getInfo(), graph.findVertexByCode(location2)->getInfo(), dd, dw)) {
            std::cerr << "[ERROR] Cannot build edge: from " << location1 << " to " << location2 << ", with dd " << dd << " and dw " << dw
                      << std::endl;
        }

        else {
            edgeCounter++;
        }

        if (!graph.addEdge(graph.findVertexByCode(location2)->getInfo(), graph.findVertexByCode(location1)->getInfo(), dd, dw)) {
            std::cerr << "[ERROR] Cannot build edge: from " << location2 << " to " << location1 << ", with dd " << dd << " and dw " << dw
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
    if (buildGraph("../data-set/Locations.csv", "../data-set/Distances.csv") == 1) {
        return; // cannot read files
    }
    std::cout << "Graph built successfully.\n";

    getMenuOptions();
}