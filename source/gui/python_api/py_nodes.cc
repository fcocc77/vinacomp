#include <py_nodes.h>
#include <node_graph.h>

static node_graph *_node_graph;

void py_nodes::init_module(QWidget *__node_graph)
{
    _node_graph = dynamic_cast<node_graph *>(__node_graph);
    static string module_name = "nodes";

    static struct PyMethodDef methods[] = {
        {"create_node", py_nodes::create_node, METH_VARARGS, "Show a number"},
        {NULL, NULL, 0, NULL}};

    static struct PyModuleDef modDef = {
        PyModuleDef_HEAD_INIT, module_name.c_str(), NULL, -1, methods,
        NULL, NULL, NULL, NULL};

    auto _module = []() {
        return PyModule_Create(&modDef);
    };

    PyImport_AppendInittab(module_name.c_str(), _module);
}

PyObject *py_nodes::create_node(PyObject *self, PyObject *args)
{
    PyObject *a;
    if (PyArg_UnpackTuple(args, "", 1, 1, &a))
    {
        _node_graph->get_maker()->create_fx("grade");
    }

    return PyLong_FromLong(20);
}
