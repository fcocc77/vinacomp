#include <general_settings.h>
#include <global.h>
#include <group_panel.h>
#include <knob_choice.h>
#include <knob_file.h>
#include <knob_text.h>
#include <os.h>
#include <panels_layout.h>
#include <script_editor.h>
#include <trim_panel.h>
#include <util.h>
#include <vinacomp.h>

group_panel::group_panel(nodes_load *_nodes_loaded)
    : nodes_loaded(_nodes_loaded)
{
}

group_panel::~group_panel() {}

void group_panel::changed(knob *_knob)
{
    QString name = _knob->get_name();

    if (name == "export")
        export_plugin_in_py_plugins();
    else if (name == "edit_script")
        edit_script();
    else if (name == "export_to_another")
        export_to_another_dir();

    plugin_panel::changed(_knob);
}

void group_panel::close()
{
    script_editor *_script_editor =
        static_cast<vinacomp *>(_vinacomp)->get_script_editor();

    _script_editor->exit_group_edit();
}

void group_panel::edit_script()
{
    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);

    panels_layout *_panels_layout = vina->get_panels_layout();
    script_editor *_script_editor = vina->get_script_editor();

    _script_editor->set_group_edit(this);
    _panels_layout->open_script_editor();
}

QJsonObject group_panel::get_child_nodes() const
{
    QJsonObject child_nodes;
    QJsonObject nodes = project->get_project_json().value("nodes").toObject();

    for (QString node_name : nodes.keys())
    {
        QString group_name = name + '.';

        if (node_name.indexOf(group_name) == 0)
        {
            QJsonObject node_obj = nodes.value(node_name).toObject();

            QString tmp_group_name = "{{group_name}}";

            QString new_node_name =
                tmp_group_name + "." + node_name.section(group_name, 1);

            node_obj["inputs"] = project->replace_parent_name_to_inputs(
                node_obj["inputs"].toObject(), name, tmp_group_name);

            node_obj["params"] = project->replace_parent_name_to_params(
                node_obj["params"].toObject(), name, tmp_group_name);

            if (node_obj.contains("handler_nodes"))
                node_obj["handler_nodes"] =
                    project->replace_parent_name_to_handlers(
                        node_obj["handler_nodes"].toArray(), name,
                        tmp_group_name);

            child_nodes.insert(new_node_name, node_obj);
        }
    }

    return child_nodes;
}

QJsonObject group_panel::get_params() const
{
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);
    QJsonObject params;

    QStringList exclusive_group_params = {"name", "icon", "group",
                                          "group_icon"};

    for (QString key : panel->get_params()->keys())
    {
        QJsonValue value = panel->get_params()->value(key);
        bool add = true;

        for (QString exclusive_param : exclusive_group_params)
            if (key.contains(exclusive_param))
            {
                add = false;
                break;
            }

        if (add)
            params.insert(key, value);
    }

    QString tmp_group_name = "{{group_name}}";
    params = project->replace_parent_name_to_params(params, get_name(),
                                                    tmp_group_name);

    return params;
}

bool group_panel::check_name_and_group_name(QString *plugin_name_pointer,
                                            QString *group_name_pointer) const
{
    QString plugin_name =
        static_cast<knob_text *>(get_knob("name"))->get_value().simplified();

    QString group_name =
        static_cast<knob_text *>(get_knob("group"))->get_value().simplified();

    if (plugin_name.isEmpty())
    {
        QMessageBox::warning(_vinacomp, "Plugin Exporter",
                             "! Plugin Name is empty", QMessageBox::Ok);
        return false;
    }
    else if (group_name.isEmpty())
    {
        QMessageBox::warning(_vinacomp, "Plugin Exporter",
                             "! Group Name is empty", QMessageBox::Ok);
        return false;
    }

    if (plugin_name_pointer)
        *plugin_name_pointer = plugin_name;

    if (group_name_pointer)
        *group_name_pointer = group_name;

    return true;
}

void group_panel::export_to_another_dir()
{
    if (!check_name_and_group_name())
        return;

    file_dialog *dialog = static_cast<vinacomp *>(_vinacomp)->get_file_dialog();

    dialog->set_save_mode();
    dialog->set_dir_mode();

    if (!dialog->exec())
        return;

    QString dir = dialog->get_files().first();
    export_plugin(dir);
}

void group_panel::export_plugin_in_py_plugins()
{
    export_plugin(PY_PLUGINS_PATH);
}

void group_panel::export_plugin(QString plugin_path)
{
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);

    QString plugin_name, group_name;
    if (!check_name_and_group_name(&plugin_name, &group_name))
        return;

    QString icon_path = static_cast<knob_file *>(get_knob("icon"))->get_value();

    QString group_icon_path =
        static_cast<knob_file *>(get_knob("group_icon"))->get_value();

    QString base_path = plugin_path + "/";

    // copiado de icono
    QString plugin_icon = base_path + plugin_name + ".png";

    if (!icon_path.isEmpty())
        os::copy(icon_path, plugin_icon);

    if (!os::isfile(plugin_icon))
        plugin_icon = "default_icon";

    if (!group_icon_path.isEmpty())
        os::copy(group_icon_path, base_path + group_name + ".png");

    // copia los custom_knobs
    QJsonObject plugin;
    plugin["group"] = group_name;
    plugin["id"] = plugin_name;
    plugin["plugin"] = true;
    plugin["script"] = base_path + plugin_name + ".py";
    plugin["label"] = plugin_name;
    plugin["icon"] = plugin_icon;
    plugin["knobs"] = *panel->custom_knobs;
    plugin["params"] = get_params();
    plugin["nodes"] = get_child_nodes();

    jwrite(base_path + plugin_name + ".json", plugin);

    // crea script para el nodo
    fwrite(base_path + plugin_name + ".py", script);

    nodes_loaded->update_py_plugins();

    static_cast<vinacomp *>(_vinacomp)
        ->get_general_settings()
        ->update_plugin_tree();
}
