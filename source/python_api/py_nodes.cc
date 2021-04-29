#include <py_nodes.h>
#include <python_api.h>

#ifdef GUI
    #include "../gui/node_graph/node.h"
    #include <node_graph.h>

    static node_graph *_node_graph;

    void py_nodes::init_module(QWidget *__node_graph)
    {
        _node_graph = static_cast<node_graph *>(__node_graph);
        init_methods();
    }
#elif ENGINE
    nodes *_nodes;

    void py_nodes::init_module(QJsonObject *project, nodes *__nodes)
    {
        _nodes = __nodes;
        init_methods();
    }
#endif

void py_nodes::init_methods()
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[6];

    init_py_module("__py_nodes__", methods,
                   {{"create_node", py_nodes::create_node},
                    {"delete_node", py_nodes::delete_node},
                    {"set_position", py_nodes::set_position},
                    {"rename", py_nodes::rename},
                    {"node_exists", py_nodes::node_exists}});
}

PyObject *py_nodes::create_node(PyObject *self, PyObject *args)
{
    const char *node_id;

    if (!PyArg_ParseTuple(args, "s", &node_id))
        return 0;

    QString name;
    #ifdef GUI
        name = _node_graph->get_maker()->create_fx(node_id)->get_name();
    #elif ENGINE
        name = node_id;
        _nodes->create_node(node_id);
    #endif

    return py_string(name);
}

PyObject *py_nodes::delete_node(PyObject *self, PyObject *args)
{
    char *name;

    if (!PyArg_ParseTuple(args, "s", &name))
        return 0;

    print("delete: " + QString(name));

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

    #ifdef GUI
        node *_node = _node_graph->get_node_view()->get_node(name);
        if (_node)
            _node->set_position(x, y);
    #endif

    return py_bool(true);
}

PyObject *py_nodes::rename(PyObject *self, PyObject *args)
{
    const char *name;
    const char *to_name;

    if (!PyArg_ParseTuple(args, "ss", &name, &to_name))
        return 0;

    #ifdef GUI
        node *_node = _node_graph->get_node_view()->get_node(name);
        if (_node)
        {
            _node_graph->get_node_view()->rename_node(_node, name, to_name);
            return py_bool(true);
        }
    #endif

    return py_bool(false);
}

PyObject *py_nodes::node_exists(PyObject *self, PyObject *args)
{
    const char *name;
    if (!PyArg_ParseTuple(args, "s", &name))
        return 0;

    bool exist = false;
    #ifdef GUI
        exist = _node_graph->get_node_view()->get_nodes()->contains(name);
    #endif

    return py_bool(exist);
}
