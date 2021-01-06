pkill -9 gdb

function run_gui() {

	# crea la paleta de colores y genera el css a partir del sass
	python3 'scripts/make_palette.py'
    npm run d

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
		log_file='/tmp/vinacomp.log'

		gdb -ex "set logging file $log_file" \
			-ex "set logging redirect on" \
		   	-ex "set confirm off" \
			-ex "set pagination off" \
		   	-ex r \
			-ex "set logging on" \
			-ex bt \
			-ex q \
			"$vinacomp" &> /dev/null &
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
        $vinaengine modules/engine_test.py
    fi
}

if [ $1 == engine ]; then
    run_engine
else
    run_gui
fi
