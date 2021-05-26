#include <node_plugin_gui.h>
#include <util.h>
#include <knob_button.h>
#include <vinacomp.h>
#include <button.h>
#include <maker.h>

node_plugin_gui::node_plugin_gui(QString script_path)
    : open_script(false)
    , is_plugin(!script_path.isEmpty())
{
    if (is_plugin)
        script = fread(script_path);
    else
        script = "def main(node, param):\n    None";
}

node_plugin_gui::~node_plugin_gui() {}

void node_plugin_gui::setup_knobs(QMap<QString, QVBoxLayout *> layouts)
{
    if (is_plugin)
    {
        get_knob("convert_to_group")->set_visible(true);
        return;
    }

    QString _script = project->nodes.value(name).script;
    if (!_script.isEmpty())
        script = _script;
}

void node_plugin_gui::changed(knob *_knob)
{
    QString name = _knob->get_name();

    if (name == "convert_to_group")
        convert_to_group();

    QStringList no_run_script_knobs = {"edit_script", "convert_to_group"};

    if (!no_run_script_knobs.contains(name))
        run_script(_knob->get_node_name(), name);
}

void node_plugin_gui::run_script(QString node_name, QString param_name)
{
    script_editor *_script_editor =
        static_cast<vinacomp *>(_vinacomp)->get_script_editor();

    QString exec = "___node = vina.get_node(\"" + node_name +
                   "\");\nmain(___node, ___node.get_param(\"" + param_name +
                   "\") )";

    if (open_script)
    {
        _script_editor->python_run("del ___node; del main");
        _script_editor->run_script_from_editor(false);
        _script_editor->run_script(exec, false, false);
    }
    else
    {
        _script_editor->python_run(script);
        _script_editor->run_script(exec, true);
    }
}

void node_plugin_gui::convert_to_group()
{
    node_view *__node_view = static_cast<node_view *>(_node_view);

    node *_node = static_cast<node *>(this_node);
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);

    node_struct props;
    props.name = name + "_group";
    props.type = "group";
    props.color = _node->get_color();
    props.params = nullptr;
    props.script = script;

    node *group = __node_view->create_node(props, true);

    group->set_position(_node->x() + _node->get_size().width() + 30,
                        _node->y());

    node_struct &__node = project->nodes[group->get_name()];
    *__node.custom_knobs = panel->base_knobs;
    *__node.params = *panel->get_params();
}
