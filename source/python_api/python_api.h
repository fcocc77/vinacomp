#ifndef PYTHON_API_H
#define PYTHON_API_H

#include <QList>
#include <python.h>
#include <util.h>

static void
init_py_module(string _module_name, PyMethodDef *methods,
               QList<pair<const char *, PyObject *(*)(PyObject *, PyObject *)>> _methods)
{
    static string module_name = _module_name;

    // define todos los metodos para el modulo
    for (int i = 0; i < _methods.count(); i++)
    {
        auto func_name = _methods[i].first;
        auto func = _methods[i].second;

        methods[i] = {func_name, func, METH_VARARGS, ""};
    }
    //
    //

    // define los modulos con los metodos definido arriba
    static struct PyModuleDef modDef = {
        PyModuleDef_HEAD_INIT, module_name.c_str(), NULL, -1, methods, NULL, NULL, NULL, NULL};
    //
    //

    // funcion que retorna el modulo creado, para poder importarlo
    auto _module = []() { return PyModule_Create(&modDef); };
    //
    //

    // importa el modulo creado a vinacomp, para que al iniciar vinacomp
    // el modulo ya este cargado.
    PyImport_AppendInittab(module_name.c_str(), _module);
}

static PyObject *py_bool(bool _bool)
{
    return PyBool_FromLong(_bool);
}

static PyObject *py_int(int _int)
{
    return PyLong_FromLong(_int);
}

static PyObject *py_float(double _double)
{
    return PyFloat_FromDouble(_double);
}

static PyObject *py_string(QString _string)
{
    return PyUnicode_FromString(_string.toStdString().c_str());
}

static PyObject *py_color(float r, float g, float b, float a = 0)
{
    PyObject *rgb_list = PyList_New(4);

    PyObject *red = Py_BuildValue("f", r);
    PyObject *green = Py_BuildValue("f", g);
    PyObject *blue = Py_BuildValue("f", b);
    PyObject *alpha = Py_BuildValue("f", a);

    PyList_SetItem(rgb_list, 0, red);
    PyList_SetItem(rgb_list, 1, green);
    PyList_SetItem(rgb_list, 2, blue);
    PyList_SetItem(rgb_list, 3, alpha);

    return rgb_list;
}

#endif // PYTHON_API_H
