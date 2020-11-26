#ifndef PY_NODES_H
#define PY_NODES_H

#include <QJsonObject>
#include <python.h>

#include <util.h>

struct py_nodes
{
    static void init_module(QJsonObject *project);

    static PyObject *create_node(PyObject *self, PyObject *args);
    static PyObject *delete_node(PyObject *self, PyObject *args);
    static PyObject *set_position(PyObject *self, PyObject *args);
    static PyObject *rename(PyObject *self, PyObject *args);
};

#endif // PY_NODES_H