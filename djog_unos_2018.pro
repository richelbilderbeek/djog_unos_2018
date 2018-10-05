# Builds the entire project

################################################################################
# Files
################################################################################
# Source code of the project
include(djog_unos_2018.pri)
# Entry point for this project
SOURCES += main.cpp

################################################################################
# Personal build
###########################r#####################################################
# Do things that depend on which you computer is used
# I (RJCB) do not think this is relevant, but would
# you need it, here you go
message($$QMAKE_HOST.name)
contains(QMAKE_HOST.name, "lubuntu"):{
  message("Welcome back Richel")
}
contains(QMAKE_HOST.name, "fwnbiol"):{
  message("Welcome back at the university")
}

################################################################################
# Compiling, linking and tools
################################################################################
# C++14
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# High warning levels
# SFML goes bad with -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror

unix:!macx {
  # Fix error: unrecognized option '--push-state--no-as-needed'
  QMAKE_LFLAGS += -fuse-ld=gold
}

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

################################################################################
# SFML
################################################################################
# GNU/Linux
unix:!macx {
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}

win32{
  # Some people use C:, others use D:, it does not hurt to put both here
  INCLUDEPATH += C:\Qt\sfml\include
  INCLUDEPATH += D:\Qt\sfml\include
  LIBS += -LC:\Qt\sfml\lib
  LIBS += -LD:\Qt\sfml\lib

  CONFIG(release, debug|release):
  {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
  }

  CONFIG(debug, debug|release):
  {
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
  }

  LIBS += -lopenal32
  LIBS += -lfreetype
  LIBS += -lopengl32
  LIBS += -lgdi32
  LIBS += -lwinmm
}


################################################################################
# Qt5
################################################################################
QT += core gui

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable

# Fixes
#/usr/include/boost/math/constants/constants.hpp:277: error: unable to find numeric literal operator 'operator""Q'
#   BOOST_DEFINE_MATH_CONSTANT(half, 5.000000000000000000000000000000000000e-01, "5.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e-01")
#   ^
QMAKE_CXXFLAGS += -fext-numeric-literals

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable

HEADERS +=

DISTFILES +=
