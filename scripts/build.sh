pkill -9 vinacomp

npm run d
rm gui/Makefile

qmake-qt5
make -j 4

gui/release/vinacomp
