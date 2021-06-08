#ifndef PY_NODE_GRAPH_H
#define PY_NODE_GRAPH_H

#include <QJsonObject>
#include <QWidget>

#include <python.h>
#include <util.h>

struct py_node_graph
{
#ifdef GUI
    static void init_module(QWidget *_vinacomp);
#endif
    static void init_methods();

    static PyObject *get_nodes_list(PyObject *self, PyObject *args);
    static PyObject *node_graph_exists(PyObject *self, PyObject *args);
    static PyObject *create_node(PyObject *self, PyObject *args);
};

#endif // PY_NODE_GRAPH_H
