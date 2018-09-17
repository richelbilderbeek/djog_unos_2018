include(djog_unos_2018.pri)
include(djog_unos_2018_test.pri)

SOURCES += main_test.cpp


# Boost.Test
LIBS += -lboost_unit_test_framework

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable

# Debug and release settings
CONFIG += debug_and_release
CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # GNU/Linux
  unix:!macx {
    # gprof
    QMAKE_CXXFLAGS += -pg
    QMAKE_LFLAGS += -pg
  }
}

CONFIG(debug, debug|release) {

  # GNU/Linux
  unix:!macx {
    # gcov
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    LIBS += -lgcov

    # helgrind, for helgrind and memcheck
    QMAKE_LFLAGS += -pthread -Wl,--no-as-needed

    # UBSAN
    QMAKE_CXXFLAGS += -fsanitize=undefined
    QMAKE_LFLAGS += -fsanitize=undefined
    LIBS += -lubsan
  }
}

# C++14
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# Fix error: unrecognized option '--push-state--no-as-needed'
QMAKE_LFLAGS += -fuse-ld=gold

# High warning level, warnings are errors
# SFML goes bad with -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
QMAKE_CXXFLAGS += -Werror

# Qt
QT += core gui

# SFML (will probably be removed in the future)
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Fixes
#/usr/include/boost/math/constants/constants.hpp:277: error: unable to find numeric literal operator 'operator""Q'
#   BOOST_DEFINE_MATH_CONSTANT(half, 5.000000000000000000000000000000000000e-01, "5.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e-01")
#   ^
QMAKE_CXXFLAGS += -fext-numeric-literals

# SFML
#LIBS += -L"/home/rafayel/SFML/lib"

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

#INCLUDEPATH += "/home/rafayel/SFML/include"
#DEPENDPATH += "/home/rafayel/SFML/include"
