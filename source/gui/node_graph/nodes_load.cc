#include <node_graph.h>
#include <nodes_bar.h>
#include <nodes_load.h>
#include <os.h>
#include <global.h>

nodes_load::nodes_load(QWidget *__node_graph)
    : _node_graph(__node_graph)
{
    QString json_nodes_path = "source/engine/nodes/json";

    // Carga todos los efectos del directorio
    for (QString effect_path : os::listdir(json_nodes_path))
    {
        QJsonObject effect = jread(effect_path);
        QString effect_id = effect.value("id").toString();

        effects.insert(effect_id, effect);
    }
    //
    //

    ofx = new ofx_api;
    for (QJsonObject effect : ofx->get_plugins())
    {
        QString effect_id = effect.value("id").toString();
        effects.insert(effect_id, effect);
    }

    load_py_plugins();
}

nodes_load::~nodes_load() {}

void nodes_load::load_py_plugins()
{
    // cargar nodos py_plugins
    QString py_plugins_dir = PY_PLUGINS_PATH;

    QStringList icons;
    for (QString file : os::listdir(py_plugins_dir))
    {
        QString basename = os::basename(file);
        QString ext = basename.split(".").last();
        QString name = basename.split(".").first();

        if (ext == "png")
            icons.push_back(name);
    }

    for (QString file : os::listdir(py_plugins_dir))
    {
        QString basename = os::basename(file);
        QString ext = basename.split(".").last();
        QString name = basename.split(".").first();

        if (ext != "json")
            continue;

        QJsonObject effect = jread(file);
        QString effect_id = effect.value("id").toString();
        QString group = effect.value("group").toString();

        QString group_icon = "default_icon";
        if (icons.contains(group))
            group_icon = py_plugins_dir + "/" + name + ".png";

        py_plugins_groups.insert(group, {group, group_icon});

        effects.insert(effect_id, effect);
        py_plugins.push_back(effect_id);
    }
}

void nodes_load::update_py_plugins()
{
    for (QString effect : py_plugins)
        effects.remove(effect);

    py_plugins.clear();
    py_plugins_groups.clear();

    load_py_plugins();

    static_cast<node_graph *>(_node_graph)
        ->get_nodes_bar()
        ->update_py_plugins();
}

QJsonObject nodes_load::get_effect(QString id) const
{
    return effects.value(id).toObject();
}

QJsonObject nodes_load::get_effects(QString group) const
{
    if (group.isEmpty())
        return effects;

    QJsonObject group_effects;
    for (QJsonValue value : effects)
    {
        QJsonObject effect = value.toObject();
        if (effect["group"].toString() == group)
            group_effects.insert(effect["id"].toString(), effect);
    }

    return group_effects;
}
