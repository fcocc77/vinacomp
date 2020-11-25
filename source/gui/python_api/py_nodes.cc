#include <py_nodes.h>
#include <node_graph.h>
#include <python_api.h>

static node_graph *_node_graph;

void py_nodes::init_module(QWidget *__node_graph)
{
    _node_graph = dynamic_cast<node_graph *>(__node_graph);

    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[3];

    init_py_module("v_nodes", methods,
                   {{"create_node", py_nodes::create_node},
                    {"delete_node", py_nodes::delete_node}});
}

PyObject *py_nodes::create_node(PyObject *self, PyObject *args)
{
    const char *node_id;

    // 1 string : s
    // 2 enteros y 1 string: lls

    if (!PyArg_ParseTuple(args, "s", &node_id))
        return 0;

    _node_graph->get_maker()->create_fx(node_id);

    // return py_int(20);
    return py_float(20.212);
}

PyObject *py_nodes::delete_node(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;

    print("delete");

    // return py_bool(true);
    return py_string("string");
}