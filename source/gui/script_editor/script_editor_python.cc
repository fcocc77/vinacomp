#include <script_editor.h>
#include <py_app.h>
#include <py_nodes.h>
#include <py_params.h>
#include <py_node_graph.h>
#include <path_utils.h>

void script_editor::python_initialize()
{
    py_app::init_module();
    py_nodes::init_module(node_graph);
    py_params::init_module(node_graph);
    py_node_graph::init_module(_vinacomp);

    Py_Initialize();
    python_module = PyImport_AddModule("__main__");

    // este es el código de Python para redirigir stdouts / stderr
    std_out_err = "class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
";

    python_run("import sys");
    python_run("sys.path.insert(1, './modules')");
    python_run("from init import *");
    python_run("import importlib");
}

QString script_editor::python_run(QString command)
{
    // invocar código para redirigir
    PyRun_SimpleString(std_out_err.c_str());

    // invocar comando
    string code = command.toStdString();
    PyRun_SimpleString(code.c_str());

    // obtener nuestro catchOutErr creado arriba
    PyObject *catcher = PyObject_GetAttrString(python_module, "catchOutErr");

    PyObject *output_obj = nullptr, *encoded = nullptr;
    if (catcher)
        // Obtener el stdout y stderr de nuestro catchOutErr
        output_obj = PyObject_GetAttrString(catcher, "value");

    if (output_obj)
        encoded = PyUnicode_AsEncodedString(output_obj, "utf-8", "strict");

    if (encoded)
        return PyBytes_AsString(encoded);

    return "";
}

void script_editor::append_sys_path(QString path)
{
    QString cmd = "if not '%1' in sys.path: sys.path.append('%1')";
    cmd = cmd.arg(path);

    python_run(cmd);
}

void script_editor::load_module(QString module_path)
{
    QString plugin_path = os::dirname(module_path);
    QString module_name = os::basename(path_util::remove_ext(module_path));

    append_sys_path(plugin_path);

    python_run("import " + module_name);
    python_run("importlib.reload(" + module_name + ")");

    append_output("Module '" + module_name + "' Imported.", {100, 150, 200});
}
