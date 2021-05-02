#include <nodes_load.h>
#include <os.h>
#include <ofxCore.h>

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
    load_ofx_plugins();
}

nodes_load::~nodes_load() {}

void nodes_load::load_ofx_plugins()
{
    int num = OfxGetNumberOfPlugins();

    OfxHost *ofx_host = new OfxHost;

    QString cimg_resources = "plugins/CImg.ofx.bundle/Contents/Resources/";
    for (int i = 0; i < num; i++)
    {
        OfxPlugin *plug = OfxGetPlugin(i);
        plug->setHost(ofx_host);
        QString effect_id = plug->pluginIdentifier;

        QString icon_path = cimg_resources + effect_id + ".png";
        QJsonObject effect = {{"group", "cimg"},
                              {"id", effect_id},
                              {"label", effect_id},
                              {"icon", icon_path}};

        effects.insert(effect_id, effect);
    }
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
