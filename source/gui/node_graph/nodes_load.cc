#include <nodes_load.h>
#include <os.h>

nodes_load::nodes_load()
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

    // cargar nodos py_plugins
    QString py_plugins = "plugins/py_plugins";

    QStringList icons;
    for (QString file : os::listdir(py_plugins))
    {
        QString basename = os::basename(file);
        QString ext = basename.split(".").last();
        QString name = basename.split(".").first();

        if (ext == "png")
            icons.push_back(name);

        if (ext != "json")
            continue;

        QJsonObject effect = jread(file);
        QString effect_id = effect.value("id").toString();
        QString group = effect.value("group").toString();

        py_plugins_groups.insert(group, {group, ""});

        effects.insert(effect_id, effect);
    }
    //

    // agrea el icono a los grupos de py_plugins si es que existe algun png con
    // el mismo nombre
    for (QString icon_name : icons)
    {
        if (py_plugins_groups.contains(icon_name))
            py_plugins_groups[icon_name].icon =
                py_plugins + "/" + icon_name + ".png";
    }
}

nodes_load::~nodes_load() {}

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
