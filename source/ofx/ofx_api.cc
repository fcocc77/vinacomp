#include <ofx_api.h>
#include <util.h>
#include <os.h>
#include <QLibrary>

ofx_api::ofx_api()
{
    load("plugins/CImg.ofx.bundle/Contents/Linux-x86-64/CImg.ofx");
    get_json_plugin("net.sf.cimg.CImgBlur");
}

void ofx_api::load(QString ofx_file)
{
    // carga todos los efectos contenidos en un binario '.ofx' y los agrega en
    // la lista de plugins
    QLibrary lib(ofx_file);

    if (!lib.load())
    {
        print(lib.errorString());
        return;
    }

    // carga las 2 funciones de ofx necesarias
    typedef int (*get_count_proto)();
    get_count_proto get_count =
        (get_count_proto)lib.resolve("OfxGetNumberOfPlugins");

    typedef OfxPlugin *(*get_plugin_proto)(int index);
    get_plugin_proto get_plugin = (get_plugin_proto)lib.resolve("OfxGetPlugin");

    if (!get_count || !get_plugin)
        return;
    //

    QString resources = os::dirname(os::dirname(ofx_file)) + "/Resources";
    QString binary_name = os::basename(ofx_file).split(".").first();

    QString binary_icon = resources + "/" + binary_name + ".png";
    if (!os::isfile(binary_icon))
        binary_icon = "default_icon";

    for (int i = 0; i < get_count(); i++)
    {
        OfxPlugin *plug = get_plugin(i);
        plug->setHost(host);
        QString plugin_id = plug->pluginIdentifier;

        QString icon_path = resources + "/" + plugin_id + ".png";
        if (!os::isfile(icon_path))
            icon_path = "default_icon";

        ofx_plugin plugin = {plug, icon_path, binary_name, binary_icon};

        plugins.insert(plug->pluginIdentifier, plugin);
    }

    binaries.push_back({binary_name, binary_icon});

    lib.unload();
}

QJsonObject ofx_api::get_json_plugin(QString ofx_id) const
{
    // transforma un plugin ofx a una estructura de 'json' que es el estandar de
    // 'vinacomp'

    ofx_plugin plugin = plugins.value(ofx_id);
    if (!plugin.plugin)
        return {};

    return {{"group", plugin.group_name},
            {"id", ofx_id},
            {"label", ofx_id},
            {"icon", plugin.icon}};
}

QList<QJsonObject> ofx_api::get_plugins() const
{
    QList<QJsonObject> _plugins;

    for (ofx_plugin plugin : plugins)
    {
        QString id = plugin.plugin->pluginIdentifier;
        _plugins.push_back(get_json_plugin(id));
    }

    return _plugins;
}
