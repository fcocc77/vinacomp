#ifndef PY_APP_H
#define PY_APP_H

#include <QJsonObject>

#include <python.h>
#include <util.h>

#ifdef ENGINE
#include <app.h>
#endif

struct py_app
{
#ifdef ENGINE
    static void init_module(QJsonObject *project, app *__app);
#elif GUI
    static void init_module();
#endif

    static void init_methods();
    static PyObject *open_project(PyObject *self, PyObject *args);
    static PyObject *save_project(PyObject *self, PyObject *args);
};

#endif // PY_APP_H