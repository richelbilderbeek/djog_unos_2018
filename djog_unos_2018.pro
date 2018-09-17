include(djog_unos_2018.pri)

# Entry point for user
SOURCES += main.cpp

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

# High warning level, warnings are errors
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
QMAKE_CXXFLAGS += -Werror

# SFML
# GNU/Linux
unix:!macx {
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}

win32{
  #LIBS += -L"/home/rafayel/SFML/lib"
  #INCLUDEPATH += "/home/rafayel/SFML/include"
  #DEPENDPATH += "/home/rafayel/SFML/include"
  INCLUDEPATH += C:/sfml/include
  DEPENDPATH += C:/sfml/include
  LIBS += -LC:/sfml/lib
  LIBS += -LC:/sfml/extlibs/libs-mingw/x86 #If using 64-bit MinGW, replace x86 with x64

  #Release Configuration
  CONFIG(debug, debug|release):
  {
    #Audio Related Libs
    LIBS += -lsfml-audio-d

    #SFML-Graphics Libs
    LIBS += -lsfml-graphics-d

    #SFML-Network Libs
    #LIBS += -lsfml-network-d

    #SFML-Window Libs
    LIBS += -lsfml-window-d

    #SFML-System Libs
    LIBS += -lsfml-system-d
  }

  #Audio Related Libs
  LIBS += -lsfml-audio
  LIBS += -lopenal32              #Dependency
  LIBS += -lFLAC                  #Dependency
  LIBS += -lvorbisenc             #Dependency
  LIBS += -lvorbisfile            #Dependency
  LIBS += -lvorbis                #Dependency
  LIBS += -logg                   #Dependency

  #SFML-Graphics Libs
  LIBS += -lsfml-graphics
  LIBS += -lfreetype              #Dependency
  #LIBS += -ljpeg                  #Dependency

  #SFML-Network Libs
  LIBS += -lsfml-network
  LIBS += -lws2_32                #Dependency

  #SFML-Window Libs
  LIBS += -lsfml-window
  LIBS += -lopengl32              #Dependency
  LIBS += -lgdi32                 #Dependency

  #SFML-System Libs
  LIBS += -lsfml-system
  LIBS += -lwinmm                 #Dependency
}

# Qt5
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
