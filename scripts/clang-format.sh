# habilita el llvm 7.0, para que funcione clang-format 7
source /opt/rh/llvm-toolset-7.0/enable

# da formato a todos los archivos .cc y .h de la carpeta 'source'.
# el archivo de cofiguracion de clang-format es '.clang-format'
# clang-format -i -style=file source/**/*.h
clang-format -i -style=file source/*/*.cc source/*/*.h
clang-format -i -style=file source/*/*/*.cc source/*/*/*.h

exit
