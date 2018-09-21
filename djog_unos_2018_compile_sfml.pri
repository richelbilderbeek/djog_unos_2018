# Compiles SFML

# Here you can do specific setups
# I (RJCB) do not think these are needed,
# but would you disagree, here is how to use them
message($$QMAKE_HOST.name)
contains(QMAKE_HOST.name, "lubuntu"):{
  message("Compiling SFML for Richel under Linux")
}
contains(QMAKE_HOST.name, "fwnbiol"):{
  message("Compiling SFML at the university")
}

# GNU/Linux
unix:!macx {
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}

win32{
  INCLUDEPATH += C:/Qt/sfml/include
  DEPENDPATH += C:/Qt/sfml/include
  DEPENDPATH += C:/Qt/sfml/bin
  LIBS += C:/Qt/sfml/lib

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
