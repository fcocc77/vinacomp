#include <engine.h>
#include <python_api.h>

engine::engine(QString _input_py)
    : input_py(_input_py)
{
    _app = new app();
    project = new QJsonObject();
    python_initialize();
}

engine::~engine()
{
}

void engine::python_initialize()
{
    py_app::init_module(project, _app);
    py_nodes::init_module(project);

    Py_Initialize();

    PyObject *sys_path = PySys_GetObject("path");

    // importacion de modulos de vinacomp
    PyList_Append(sys_path, py_string("./modules"));
    PyRun_SimpleString("from init import *");
    //
    //

    // importacion de archivo python del parametro entrante
    QString input_py_dir = os::dirname(input_py);
    string input_py_name = os::basename(input_py).split(".")[0].toStdString();
    string input_py_module = "from " + input_py_name + " import *";
    PyList_Append(sys_path, py_string(input_py_dir));
    PyRun_SimpleString(input_py_module.c_str());
    //
    //

    Py_Finalize();
}