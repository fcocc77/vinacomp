#include <python_api.h>
#include <py_nodes.h>

void py_nodes::init_module(QJsonObject *project)
{
    // el tamaño de la lista de metodos tiene que ser 1 mayor
    // a los metodos que existen
    static struct PyMethodDef methods[5];

    init_py_module("__py_nodes__", methods,
                   {{"create_node", py_nodes::create_node},
                    {"delete_node", py_nodes::delete_node},
                    {"set_position", py_nodes::set_position},
                    {"rename", py_nodes::rename}});
}

PyObject *py_nodes::create_node(PyObject *self, PyObject *args)
{
    const char *node_id;

    if (!PyArg_ParseTuple(args, "s", &node_id))
        return 0;

    QString name = "node_name";

    return py_string(name);
}

PyObject *py_nodes::delete_node(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;

    print("delete");

    return py_string("string");
}

PyObject *py_nodes::set_position(PyObject *self, PyObject *args)
{
    int x;
    int y;
    const char *name;

    if (!PyArg_ParseTuple(args, "sll", &name, &x, &y))
        return 0;

    return py_bool(true);
}

PyObject *py_nodes::rename(PyObject *self, PyObject *args)
{
    const char *name;
    const char *to_name;

    if (!PyArg_ParseTuple(args, "ss", &name, &to_name))
        return 0;

    return py_bool(false);
}