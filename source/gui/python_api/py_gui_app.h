#ifndef PY_GUI_APP_H
#define PY_GUI_APP_H

struct py_app
{
    static void init_module();
    static PyObject *open_project(PyObject *self, PyObject *args);
    static PyObject *save_project(PyObject *self, PyObject *args);
};



#endif //PY_GUI_APP_H