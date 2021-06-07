#ifndef PY_PARAMS_H
#define PY_PARAMS_H

#include <QJsonObject>

#include <python.h>
#include <util.h>

struct py_params
{
#ifdef GUI
    static void init_module(QWidget *__node_graph);
#endif

    static void init_methods();
    static PyObject *get_value(PyObject *self, PyObject *args);
    static PyObject *get_index(PyObject *self, PyObject *args);
    static PyObject *set_value(PyObject *self, PyObject *args);
    static PyObject *set_index(PyObject *self, PyObject *args);
    static PyObject *get_color(PyObject *self, PyObject *args);
    static PyObject *param_exists(PyObject *self, PyObject *args);

    static QWidget *get_knob(QString node_name, QString parm_name);
};

#endif // PY_PARAMS_H
