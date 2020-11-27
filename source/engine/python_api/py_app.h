#ifndef PY_APP_H
#define PY_APP_H

#include <QJsonObject>
#include <app.h>

#include <python.h>

#include <util.h>

struct py_app
{
    static void init_module(QJsonObject *project, app *__app);
    static PyObject *open_project(PyObject *self, PyObject *args);
    static PyObject *save_project(PyObject *self, PyObject *args);
};

#endif // PY_APP_H