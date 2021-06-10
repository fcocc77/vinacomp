kill -9 gdb

function run_gui() {

    if [ $1 == 'full' ]; then
        # crea la paleta de colores y genera el css a partir del sass
        python3 'scripts/make_palette.py'
        npm run d

        # cambia 'path' del proyecto para el archivo 'style.css'
        css='resources/css/style.css'
        path=$(pwd)
        sed -i "s|{{path}}|$path|g" $css
        #

        # conversion de svg a png
        sh scripts/svg_converter.sh
        #
    fi

    rm source/gui/Makefile

    pkill -9 vinacomp
    vinacomp='source/gui/release/vinacomp'
    rm $vinacomp

    qmake-qt5
    make -j 8

    # Cambiar posicion despues de 2 segundos
    sleep 3 && wmctrl -r "Untitled - VinaComp" -e 0,0,0,2560,800 &

    if [ -f $vinacomp ]; then
        if [ $1 == 'debug' ]; then
            log_file='/tmp/vinacomp.log'

            gdb -ex "set confirm off" \
                -ex "set pagination off" \
                -ex "set logging on" \
                -ex r \
                -ex bt \
                -ex q \
                "$vinacomp"
        else
            $vinacomp
        fi
    fi
}

function run_engine() {
    rm source/engine/Makefile

    pkill -9 vinaengine
    vinaengine='source/engine/release/vinaengine'
    rm $vinaengine

    qmake-qt5
    make -j 8

    if [ -f $vinaengine ]; then
        $vinaengine modules/engine_test.py
    fi
}

if [ $1 == engine ]; then
    run_engine
elif [ $1 == full ]; then
    run_gui 'full'
elif [ $1 == debug ]; then
    run_gui 'debug'
else
    run_gui 'not'
fi
