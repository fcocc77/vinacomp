#include <engine.h>

engine::engine()
{
    project = new QJsonObject();

    py_nodes::init_module(project);

    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.insert(1, './modules')");
    PyRun_SimpleString("from init import *");

    Py_Finalize();
}

engine::~engine()
{
}
