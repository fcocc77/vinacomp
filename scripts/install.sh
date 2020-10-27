install_dir='/opt/vinacomp'

function install() {
    # Compilacion
    sh scripts/build.sh

    mkdir ./bin
    cp gui/release/vinacomp ./bin/vinacomp
    #
    #

    # Copiado a la carpeta de instalacion
    mkdir $install_dir

    cp -rf ./bin $install_dir
    cp -rf ./config $install_dir
    cp -rf ./resources $install_dir
    cp -rf ./libs $install_dir

    cp ./resources/app/vinacomp.sh $install_dir/vinacomp
    cp ./resources/app/VinaComp.desktop /usr/local/share/applications/VinaComp.desktop
    #
    #

    chmod 755 -R $install_dir
}

function uninstall() {
    rm -rf $install_dir
    rm /usr/local/share/applications/VinaComp.desktop
}

uninstall
install
