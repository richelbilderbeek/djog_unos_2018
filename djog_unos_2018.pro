include(djog_unos_2018.pri)
# Entry point for user
SOURCES += main.cpp

# High warning levels
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Weffc++ -Werror

# C++14
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

