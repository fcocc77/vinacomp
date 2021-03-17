#ifndef PY_NODES_H
#define PY_NODES_H

#include <QJsonObject>
#include <python.h>

#include <util.h>

#ifdef ENGINE
#include <nodes.h>
#endif

struct py_nodes
{
#ifdef GUI
    static void init_module( QWidget *__node_graph );
#elif ENGINE
    static void init_module( QJsonObject *project, nodes *__nodes );
#endif

    static void init_methods();

    static PyObject *create_node( PyObject *self, PyObject *args );
    static PyObject *delete_node( PyObject *self, PyObject *args );
    static PyObject *set_position( PyObject *self, PyObject *args );
    static PyObject *rename( PyObject *self, PyObject *args );
    static PyObject *node_exists( PyObject *self, PyObject *args );
};

#endif // PY_NODES_H