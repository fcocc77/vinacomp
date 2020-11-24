pkill -9 vinacomp
pkill -9 gdb

vinacomp='source/gui/release/vinacomp'

npm run d
rm source/gui/Makefile
rm $vinacomp

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

if [ -f $vinacomp ]; then
    gdb -ex run $vinacomp
fi
