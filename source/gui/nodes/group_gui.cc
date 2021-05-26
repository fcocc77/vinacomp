#include <general_settings.h>
#include <group_gui.h>
#include <knob_choice.h>
#include <knob_file.h>
#include <knob_text.h>
#include <os.h>
#include <panels_layout.h>
#include <script_editor.h>
#include <trim_panel.h>
#include <util.h>
#include <vinacomp.h>
#include <global.h>

group_gui::group_gui(nodes_load *_nodes_loaded)
    : nodes_loaded(_nodes_loaded)
{
}

group_gui::~group_gui() {}

void group_gui::changed(knob *_knob)
{
    QString name = _knob->get_name();

    if (name == "export")
        export_plugin();
    else if (name == "edit_script")
        edit_script();

    node_plugin_gui::changed(_knob);
}

void group_gui::close()
{
    script_editor *_script_editor =
        static_cast<vinacomp *>(_vinacomp)->get_script_editor();

    _script_editor->exit_group_edit();
}

void group_gui::edit_script()
{
    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);

    panels_layout *_panels_layout = vina->get_panels_layout();
    script_editor *_script_editor = vina->get_script_editor();

    _script_editor->set_group_edit(this);
    _panels_layout->open_script_editor();
}

QJsonObject group_gui::get_child_nodes() const
{
    QJsonObject child_nodes;
    QJsonObject nodes = project->get_project_json().value("nodes").toObject();

    for (QString node_name : nodes.keys())
    {
        QJsonObject node_obj = nodes.value(node_name).toObject();

        QString group_name = name + '.';
        QString basename = node_name.section(group_name, 1);

        if (node_name.contains(group_name) && !basename.isEmpty())
            child_nodes.insert(basename, node_obj);
    }

    return child_nodes;
}

void group_gui::export_plugin()
{
    knob_text *name_knob = static_cast<knob_text *>(get_knob("name"));
    trim_panel *panel = static_cast<trim_panel *>(name_knob->get_panel());

    QString name = name_knob->get_value().simplified();
    QString icon_path = static_cast<knob_file *>(get_knob("icon"))->get_value();

    QString group_name =
        static_cast<knob_text *>(get_knob("group"))->get_value().simplified();
    QString group_icon_path =
        static_cast<knob_file *>(get_knob("group_icon"))->get_value();

    QString base_path = PY_PLUGINS_PATH + "/";

    if (name.isEmpty())
    {
        QMessageBox::warning(_vinacomp, "Plugin Exporter",
                             "! Plugin Name is empty", QMessageBox::Ok);
        return;
    }
    else if (group_name.isEmpty())
    {
        QMessageBox::warning(_vinacomp, "Plugin Exporter",
                             "! Group Name is empty", QMessageBox::Ok);
        return;
    }

    // copiado de icono
    QString plugin_icon = base_path + name + ".png";
    os::copy(icon_path, plugin_icon);

    if (!os::isfile(plugin_icon))
        plugin_icon = "default_icon";

    os::copy(group_icon_path, base_path + group_name + ".png");

    // copia los custom_knobs
    QJsonObject plugin;
    plugin["group"] = group_name;
    plugin["id"] = name;
    plugin["plugin"] = true;
    plugin["script"] = base_path + name + ".py";
    plugin["label"] = name;
    plugin["icon"] = plugin_icon;
    plugin["knobs"] = *panel->custom_knobs;
    plugin["nodes"] = get_child_nodes();

    jwrite(base_path + name + ".json", plugin);

    // crea script para el nodo
    fwrite(base_path + name + ".py", script);

    nodes_loaded->update_py_plugins();

    static_cast<vinacomp *>(_vinacomp)
        ->get_general_settings()
        ->update_plugin_tree();
}
