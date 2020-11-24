#ifndef PY_NODES_H
#define PY_NODES_H

#include <Python.h>

#include <QObject>
#include <QWidget>

#include <util.h>

struct py_nodes
{
    static PyObject *create_node(PyObject *self, PyObject *args);
    static void init_module(QWidget *__node_graph);
};

#endif // PY_NODES_H