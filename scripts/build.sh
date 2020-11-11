pkill -9 vinacomp

npm run d
rm gui/Makefile

# cambia 'path' del proyecto para el archivo 'style.css'
css='resources/css/style.css'
path=$(pwd)
sed -i "s|{{path}}|$path|g" $css
#
#

# conversion de svg a png
sh scripts/svg_converter.sh
#
#

qmake-qt5
make -j 4

optirun gui/release/vinacomp
