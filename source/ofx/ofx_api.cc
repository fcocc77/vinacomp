#include <QLibrary>
#include <ofxImageEffect.h>
#include <ofx_api.h>
#include <ofx_property_suite.h>
#include <ofx_image_effect_suite.h>
#include <ofx_parameter_suite.h>
#include <os.h>
#include <util.h>

OfxPropertySuiteV1 *prop_suite = new OfxPropertySuiteV1;
OfxImageEffectSuiteV1 *effect_suite = new OfxImageEffectSuiteV1;
OfxParameterSuiteV1 *param_suite = new OfxParameterSuiteV1;

const void *fetchSuite(OfxPropertySetHandle host, const char *suiteName,
                       int suiteVersion)
{
    QString name = suiteName;

    if (name == (QString)kOfxPropertySuite)
        return prop_suite;
    else if (name == (QString)kOfxImageEffectSuite)
        return effect_suite;
    else if (name == (QString)kOfxParameterSuite)
        return param_suite;

    return host;
}

ofx_api::ofx_api()
{
    host = new OfxHost;
    host->fetchSuite = fetchSuite;

    // agrega nuestras funciones a los punteros de funciones de

    // 'OfxPropertySuiteV1'
    prop_suite->propSetPointer = prop_set_pointer;
    prop_suite->propSetString = prop_set_string;
    prop_suite->propSetDouble = prop_set_double;
    prop_suite->propSetInt = prop_set_int;
    prop_suite->propSetPointerN = prop_set_pointer_n;
    prop_suite->propSetStringN = prop_set_string_n;
    prop_suite->propSetDoubleN = prop_set_double_n;
    prop_suite->propSetIntN = prop_set_int_n;
    prop_suite->propGetPointer = prop_get_pointer;
    prop_suite->propGetString = prop_get_string;
    prop_suite->propGetDouble = prop_get_double;
    prop_suite->propGetInt = prop_get_int;
    prop_suite->propGetPointerN = prop_get_pointer_n;
    prop_suite->propGetStringN = prop_get_string_n;
    prop_suite->propGetDoubleN = prop_get_double_n;
    prop_suite->propGetIntN = prop_get_int_n;
    prop_suite->propReset = prop_reset;
    prop_suite->propGetDimension = prop_get_dimension;
    //

    // 'OfxImageEffectSuiteV1'
    effect_suite->getPropertySet = get_property_set;
    effect_suite->getParamSet = get_param_set;
    effect_suite->clipDefine = clip_define;
    effect_suite->clipGetHandle = clip_get_handle;
    effect_suite->clipGetPropertySet = clip_get_property_set;
    effect_suite->clipGetImage = clip_get_image;
    effect_suite->clipReleaseImage = clip_release_image;
    effect_suite->clipGetRegionOfDefinition = clip_get_region_of_definition;
    effect_suite->abort = abort;
    effect_suite->imageMemoryAlloc = image_memory_alloc;
    effect_suite->imageMemoryFree = image_memory_free;
    effect_suite->imageMemoryLock = image_memory_lock;
    effect_suite->imageMemoryUnlock = image_memory_unlock;
    //

    // 'OfxParameterSuiteV1'
    param_suite->paramDefine = param_define;
    param_suite->paramGetHandle = param_get_handle;
    param_suite->paramSetGetPropertySet = param_set_get_property_set;
    param_suite->paramGetPropertySet = param_get_property_set;
    param_suite->paramGetValue = param_get_value;
    param_suite->paramGetValueAtTime = param_get_value_at_time;
    param_suite->paramGetDerivative = param_get_derivative;
    param_suite->paramGetIntegral = param_get_integral;
    param_suite->paramSetValue = param_set_value;
    param_suite->paramSetValueAtTime = param_set_value_at_time;
    param_suite->paramGetNumKeys = param_get_num_keys;
    param_suite->paramGetKeyTime = param_get_key_time;
    param_suite->paramGetKeyIndex = param_get_key_index;
    param_suite->paramDeleteKey = param_delete_key;
    param_suite->paramDeleteAllKeys = param_delete_all_keys;
    param_suite->paramCopy = param_copy;
    param_suite->paramEditBegin = param_edit_begin;
    param_suite->paramEditEnd = param_edit_end;
    //

    load("plugins/ofx/CImg.ofx.bundle/Contents/Linux-x86-64/CImg.ofx");
    load("plugins/ofx/Misc.ofx.bundle/Contents/Linux-x86-64/Misc.ofx");
    // load("libs/ofx/Documentation/sources/Guide/Code/Example3/gain.ofx");
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
        plug->mainEntry(kOfxActionLoad, 0, 0, 0);
        plug->mainEntry(kOfxActionDescribe, 0, 0, 0);
        plug->mainEntry(kOfxImageEffectActionDescribeInContext, 0, 0, 0);

        QString plugin_id = plug->pluginIdentifier;

        QString icon_path = resources + "/" + plugin_id + ".png";
        if (!os::isfile(icon_path))
            icon_path = "default_icon";

        ofx_plugin plugin = {plug, icon_path, binary_name, binary_icon};

        plugins.insert(plug->pluginIdentifier, plugin);
    }

    binaries.push_back({binary_name, binary_icon});
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
