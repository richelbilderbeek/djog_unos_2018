#!/bin/sh
#
# Crosscompiles Nature Zen to the Windows operating system
#
# Usage:
#
# ./cross_compile.sh
#
 
DESKTOP_PRO=djog_unos_2018.pro

cd ..

if [ ! -e $DESKTOP_PRO ]
then
  echo "ERROR: Cannot find "$DESKTOP_PRO
  exit
fi
i686-w64-mingw32.static-qmake-qt5 $DESKTOP_PRO
make
cd ..

DESKTOP_EXE=./Desktop/release/NatureZen.exe

if [ ! -e $DESKTOP_EXE ]
then
  echo "ERROR: Cannot find "$DESKTOP_EXE
else
  echo "OK: Can find "$DESKTOP_EXE
fi


mv $DESKTOP_EXE NatureZen.exe