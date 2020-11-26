#include <py_nodes.h>
#include <node_graph.h>
#include <python_api.h>
#include <node.h>

static node_graph *_node_graph;

void py_nodes::init_module(QWidget *__node_graph)
{
    _node_graph = dynamic_cast<node_graph *>(__node_graph);

    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[4];

    init_py_module("v_nodes", methods,
                   {{"create_node", py_nodes::create_node},
                    {"delete_node", py_nodes::delete_node},
                    {"set_position", py_nodes::set_position}});
}

PyObject *py_nodes::create_node(PyObject *self, PyObject *args)
{
    const char *node_id;

    if (!PyArg_ParseTuple(args, "s", &node_id))
        return 0;

    QString name = _node_graph->get_maker()->create_fx(node_id);

    return py_string(name);
}

PyObject *py_nodes::delete_node(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;

    print("delete");

    // return py_bool(true);
    return py_string("string");
}

PyObject *py_nodes::set_position(PyObject *self, PyObject *args)
{
    int x;
    int y;
    const char *name;

    if (!PyArg_ParseTuple(args, "sll", &name, &x, &y))
        return 0;

    node *_node = _node_graph->get_node_view()->get_node(name);
    if (_node)
        _node->set_position(x, y);

    return py_bool(true);
}