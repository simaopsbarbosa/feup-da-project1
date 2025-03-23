#include <iostream>
#include <string>

using namespace std;

void show_menu() {
  cout << "Route Planning\n";
  cout << "1. Independent Route Planning\n";
  cout << "2. Restricted Route Planning\n";
  cout << "3. Environmentally-Friendly Route Planning (driving and walking)\n";
  cout << "4. Leave\n";
  cout << "Choose an option: ";
}

int main() {
  int x;

  while (true) {
    show_menu();
    cin >> x;

    switch (x) {

    case 1:
      cout << "Calculating independent route...\n";
      break;

    case 2:
      cout << "Calculating restricted route...\n";
      break;

    case 3:
      cout << "Calculating environmentally-friendly route...\n";
      break;

    case 4:
      cout << "Leaving program...\n";
      return 0;

    default:
      cout << "Invalid option. Try again.\n";
    }
  }
  return 0;
}
