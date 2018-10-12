#include <iostream>
#include <SFML/Main.hpp>

int main() {
  std::cout
    << 'v' << SFML_VERSION_MAJOR
    << "." << SFML_VERSION_MINOR
    << "." << SFML_VERSION_PATCH
  ;
}
