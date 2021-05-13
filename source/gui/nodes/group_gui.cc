#include <group_gui.h>
#include <knob_choice.h>
#include <knob_file.h>
#include <knob_text.h>
#include <os.h>
#include <trim_panel.h>
#include <util.h>

group_gui::group_gui() {}

group_gui::~group_gui() {}

void group_gui::changed(knob *_knob)
{
    QString name = _knob->get_name();

    if (name == "export")
        export_plugin();
}

void group_gui::export_plugin()
{
    knob_text *name_knob = static_cast<knob_text *>(get_knob("name"));
    trim_panel *panel = static_cast<trim_panel *>(name_knob->get_panel());

    QString name = name_knob->get_value();
    print(name);
    QString icon_path = static_cast<knob_file *>(get_knob("icon"))->get_value();

    QString group_name =
        static_cast<knob_text *>(get_knob("group"))->get_value();
    QString group_icon_path =
        static_cast<knob_file *>(get_knob("group_icon"))->get_value();

    QString base_path = "plugins/py_plugins/" + name;

    // copiado de icono
    QString plugin_icon = base_path + ".png";
    os::copy(icon_path, plugin_icon);

    // copia los custom_knobs
    QJsonObject plugin;
    plugin["group"] = group_name;
    plugin["id"] = name;
    plugin["label"] = name;
    plugin["icon"] = plugin_icon;
    plugin["knobs"] = *panel->custom_knobs;

    jwrite(base_path + ".json", plugin);
}
