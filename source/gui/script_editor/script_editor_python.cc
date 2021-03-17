#include <script_editor.h>

void script_editor::python_initialize()
{
    py_app::init_module();
    py_nodes::init_module( node_graph );

    Py_Initialize();
    python_module = PyImport_AddModule( "__main__" );

    // este es el código de Python para redirigir stdouts / stderr
    std_out_err = "class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
";

    PyRun_SimpleString( "import sys" );
    PyRun_SimpleString( "sys.path.insert(1, './modules')" );
    PyRun_SimpleString( "from init import *" );
}

QString script_editor::python_run( QString command )
{
    // invocar código para redirigir
    PyRun_SimpleString( std_out_err.c_str() );

    // invocar comando
    string code = command.toStdString();
    PyRun_SimpleString( code.c_str() );

    // obtener nuestro catchOutErr creado arriba
    PyObject *catcher = PyObject_GetAttrString( python_module, "catchOutErr" );

    PyObject *output_obj = nullptr, *encoded = nullptr;
    if ( catcher )
        // Obtener el stdout y stderr de nuestro catchOutErr
        output_obj = PyObject_GetAttrString( catcher, "value" );

    if ( output_obj )
        encoded = PyUnicode_AsEncodedString( output_obj, "utf-8", "strict" );

    if ( encoded )
        return PyBytes_AsString( encoded );

    return "";
}