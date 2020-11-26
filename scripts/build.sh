pkill -9 gdb

npm run d

function run_gui() {

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

    rm source/gui/Makefile

    pkill -9 vinacomp
    vinacomp='source/gui/release/vinacomp'
    rm $vinacomp

    qmake-qt5
    make -j 4

    if [ -f $vinacomp ]; then
        gdb -ex run $vinacomp
    fi
}

function run_engine() {
    rm source/engine/Makefile

    pkill -9 vinaengine
    vinaengine='source/engine/release/vinaengine'
    rm $vinaengine

    qmake-qt5
    make -j 4

    if [ -f $vinaengine ]; then
        $vinaengine
    fi
}

if [ $1 == engine ]; then
    run_engine
else
    run_gui
fi
