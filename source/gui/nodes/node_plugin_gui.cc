#include <node_plugin_gui.h>
#include <util.h>
#include <vinacomp.h>

node_plugin_gui::node_plugin_gui(QString script_path)
    : open_script(false)
    , is_plugin(!script_path.isEmpty())
{
    if (is_plugin)
        script = fread(script_path);
    else
        script = "def callback(node, param):\n    None";
}

node_plugin_gui::~node_plugin_gui() {}

void node_plugin_gui::setup_knobs(QMap<QString, QVBoxLayout *> layouts)
{
    if (is_plugin)
        return;

    QString _script = project->nodes.value(name).script;
    if (!_script.isEmpty())
        script = _script;
}

void node_plugin_gui::changed(knob *_knob)
{
    QString name = _knob->get_name();

    if (name != "edit_script")
        run_script(_knob->get_node_name(), name);
}

void node_plugin_gui::run_script(QString node_name, QString param_name)
{
    script_editor *_script_editor =
        static_cast<vinacomp *>(_vinacomp)->get_script_editor();

    QString exec = "___node = vina.get_node(\"" + node_name +
                   "\");\ncallback(___node, ___node.get_param(\"" + param_name +
                   "\") )";

    if (open_script)
    {
        _script_editor->run_script_from_editor(false);
        _script_editor->run_script(exec, false);
    }
    else
    {
        _script_editor->python_run(script);
        _script_editor->python_run(exec);
    }
}
