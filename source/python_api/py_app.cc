#include <python_api.h>
#include <py_app.h>

#ifdef ENGINE
    app *_app;
    void py_app::init_module(QJsonObject *project, app *__app)
    {
        _app = __app;
        init_methods();
    }
#elif GUI
    void py_app::init_module()
    {
        init_methods();
    }
#endif

void py_app::init_methods()
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[3];

    init_py_module("__py_app__", methods,
                   {{"open_project", py_app::open_project},
                    {"save_project", py_app::save_project}});
}

PyObject *py_app::open_project(PyObject *self, PyObject *args)
{
    const char *project_path;

    if (!PyArg_ParseTuple(args, "s", &project_path))
        return 0;

    #ifdef ENGINE
        _app->open_project(project_path);
    #endif

    return py_bool(true);
}

PyObject *py_app::save_project(PyObject *self, PyObject *args)
{
    const char *project_path;

    if (!PyArg_ParseTuple(args, "s", &project_path))
        return 0;

    #ifdef ENGINE
        _app->save_project(project_path);
    #endif

    return py_bool(true);
}
