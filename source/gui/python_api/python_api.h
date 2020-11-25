#ifndef PYTHON_API_H
#define PYTHON_API_H

#include <Python.h>
#include <QList>

static void init_py_module(
    string _module_name,
    PyMethodDef *methods,
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
    auto _module = []() {
        return PyModule_Create(&modDef);
    };
    //
    //

    // importa el modulo creado a vinacomp, para que al iniciar vinacomp
    // el modulo ya este cargado.
    PyImport_AppendInittab(module_name.c_str(), _module);
}

#endif // PYTHON_API_H