#include <py_node_graph.h>
#include <python_api.h>
#include <node_graph.h>
#include <vinacomp.h>

#ifdef GUI
    QWidget *_vinacomp = 0;
    void py_node_graph::init_module(QWidget *__vinacomp)
    {
        _vinacomp = __vinacomp;
        init_methods();
    }
#endif

void py_node_graph::init_methods()
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[4];

    init_py_module("__py_node_graph__", methods,
                   {{"get_nodes_list", py_node_graph::get_nodes_list},
                    {"create_node", py_node_graph::create_node},
                    {"node_graph_exists", py_node_graph::node_graph_exists}});
}

PyObject *py_node_graph::get_nodes_list(PyObject *self, PyObject *args)
{
    const char *node_graph_name;

    if (!PyArg_ParseTuple(args, "s", &node_graph_name))
        return 0;

    #ifdef GUI
        node_graph *_node_graph =
            static_cast<vinacomp *>(_vinacomp)->get_node_graph(node_graph_name);
    #endif

    return py_int(-1);
}

PyObject *py_node_graph::create_node(PyObject *self, PyObject *args)
{
    const char *node_graph_name;
    const char *node_id;
    bool basic_creation;

    if (!PyArg_ParseTuple(args, "ssb", &node_graph_name, &node_id, &basic_creation))
        return 0;

    bool created = false;

    #ifdef GUI
        node_graph *_node_graph =
            static_cast<vinacomp *>(_vinacomp)->get_node_graph(node_graph_name);
        if (_node_graph)
        {
            _node_graph->get_maker()->create_fx(node_id, basic_creation);
            created = true;
        }
    #endif

    return py_bool(created);
}

PyObject *py_node_graph::node_graph_exists(PyObject *self, PyObject *args)
{
    const char *node_graph_name;

    if (!PyArg_ParseTuple(args, "s", &node_graph_name))
        return 0;

    #ifdef GUI
    #endif

    return py_bool(true);
}
