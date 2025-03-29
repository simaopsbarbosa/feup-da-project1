#include "code/Menu.h"

int main(int argc, char *argv[]) {
  std::string locationsFile;
  std::string distancesFile;

  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " <locations_file> <distances_file> (<input_file> "
                 "(<output_file>))\n";
    std::cout << "Do you want to try to read from default "
                 "locations?\n(../data-set/Locations.csv and "
                 "../data-set/Distances.csv) (y/n) : ";
    char answer;
    std::cin >> answer;
    if (answer == 'y' || answer == 'Y') {
      locationsFile = "../data-set/Locations.csv";
      distancesFile = "../data-set/Distances.csv";
    } else {
      std::cout << "Exiting...\n";
      return 1;
    }
  } else {
    locationsFile = argv[1];
    distancesFile = argv[2];
  }
  Menu menu = Menu(locationsFile, distancesFile, argc > 3 ? argv[3] : "",
                   argc > 4 ? argv[4] : "");
  return 0;
}