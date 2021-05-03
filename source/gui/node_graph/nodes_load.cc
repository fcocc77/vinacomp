#include <nodes_load.h>
#include <os.h>
#include <ofxCore.h>
#include <QLibrary>

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

void nodes_load::load_ofx(QString ofx_name)
{
    QString ofx_path = "plugins/" + ofx_name +
                       ".ofx.bundle/Contents/Linux-x86-64/" + ofx_name + ".ofx";

    QLibrary lib(ofx_path);

    if (!lib.load())
        return;

    // carga las 2 funciones de ofx necesarias
    typedef int (*get_count_proto)();
    get_count_proto get_count =
        (get_count_proto)lib.resolve("OfxGetNumberOfPlugins");

    typedef OfxPlugin *(*get_plugin_proto)(int index);
    get_plugin_proto get_plugin = (get_plugin_proto)lib.resolve("OfxGetPlugin");

    if (!get_count || !get_plugin)
        return;
    //

    QString ofx_resources =
        "plugins/" + ofx_name + ".ofx.bundle/Contents/Resources/";

    for (int i = 0; i < get_count(); i++)
    {
        OfxPlugin *plug = get_plugin(i);
        QString effect_id = plug->pluginIdentifier;

        QString icon_path = ofx_resources + effect_id + ".png";
        QJsonObject effect = {{"group", "cimg"},
                              {"id", effect_id},
                              {"label", effect_id},
                              {"icon", icon_path}};

        effects.insert(effect_id, effect);
    }
}

void nodes_load::load_ofx_plugins()
{
    load_ofx("CImg");
    load_ofx("Misc");
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
