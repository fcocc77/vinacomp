#include <python_api.h>
#include <py_app.h>

app *_app;

void py_app::init_module(QJsonObject *project, app *__app)
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[2];

    init_py_module("__py_app__", methods,
                   {{"open_project", py_app::open_project}});
}

PyObject *py_app::open_project(PyObject *self, PyObject *args)
{
    const char *project_path;

    if (!PyArg_ParseTuple(args, "s", &project_path))
        return 0;

    _app->open_project(project_path);

    return py_bool(true);
}