#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
  std::cout
    << 'v' << SFML_VERSION_MAJOR
    << "." << SFML_VERSION_MINOR
    //<< "." << SFML_VERSION_PATCH
  ;
}
