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
