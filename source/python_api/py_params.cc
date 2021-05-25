#include <py_params.h>
#include <python_api.h>
#include <knob_floating.h>

#ifdef GUI
    #include "node.h"
    #include <node_graph.h>

    static node_graph *_node_graph;

    void py_params::init_module(QWidget *__node_graph)
    {
        _node_graph = static_cast<node_graph *>(__node_graph);
        init_methods();
    }
#endif

void py_params::init_methods()
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[4];

    init_py_module("__py_params__", methods,
                   {{"get_value", py_params::get_value},
                    {"set_value", py_params::set_value},
                    {"param_exists", param_exists}});
}

QWidget *py_params::get_knob(QString node_name, QString param_name)
{
#ifdef GUI
        node *_node = _node_graph->get_node_view()->get_node(node_name);
        if (_node)
            return _node->get_trim_panel()->get_knob(param_name);
    #endif
        return nullptr;
}

PyObject *py_params::get_value(PyObject *self, PyObject *args)
{
    const char *node_name, *param_name;

    if (!PyArg_ParseTuple(args, "ss", &node_name, &param_name))
        return 0;

    float value = 0;
    #ifdef GUI
        knob_floating *_knob =
            dynamic_cast<knob_floating *>(get_knob(node_name, param_name));
        if (_knob)
            value = _knob->get_value();
    #endif

    return py_float(value);
}

PyObject *py_params::set_value(PyObject *self, PyObject *args)
{
    const char *node_name, *param_name;
    float value;

    if (!PyArg_ParseTuple(args, "ssf", &node_name, &param_name, &value))
        return 0;

    bool changed = false;
    #ifdef GUI
        knob_floating *_knob =
            dynamic_cast<knob_floating *>(get_knob(node_name, param_name));
        if (_knob)
        {
            _knob->set_value(value);
            changed = true;
        }
    #endif

    return py_bool(changed);
}

PyObject *py_params::param_exists(PyObject *self, PyObject *args)
{
    const char *node_name, *param_name;

    if (!PyArg_ParseTuple(args, "ss", &node_name, &param_name))
        return 0;

    bool exist = false;
    #ifdef GUI
        node *_node = _node_graph->get_node_view()->get_node(node_name);
        exist = _node->get_trim_panel()->get_knobs()->contains(param_name);
    #endif

    return py_bool(exist);
}
