#include <QSplitter>

#include <button.h>
#include <general_settings.h>
#include <global.h>
#include <knob_check_box.h>
#include <knob_integer.h>
#include <setup_knobs.h>
#include <util.h>

general_settings::general_settings(QWidget *_vinacomp)
    : settings(true, _vinacomp)
    , data(new QJsonObject)
{
    setup_general();
    setup_auto_save();
    setup_plugins();
    setup_appearance();

    set_content("Plugins");
}

general_settings::~general_settings()
{
    delete data;
}

void general_settings::setup_general()
{
    QVBoxLayout *layout = add_item("General");
}

void general_settings::setup_auto_save()
{
    QVBoxLayout *layout = add_item("AutoSave");

    setup_knobs_props props;

    QJsonObject enable_knob = {{"type", "check_box"},
                               {"name", "auto_save_enable"},
                               {"label", "Enable AutoSave"},
                               {"default", true}};

    QJsonObject delay_knob = {{"type", "integer"},
                              {"name", "auto_save_delay"},
                              {"label", "Delay: Seconds"},
                              {"min", 1},
                              {"max", 30},
                              {"default", 5}};

    props.knobs_array.push_back(enable_knob);
    props.knobs_array.push_back(delay_knob);

    props.layout = layout;
    props.knobs = knobs;
    props.params = data;

    setup_knobs(props);
}

void general_settings::setup_appearance()
{
    QVBoxLayout *layout = add_item("Appearance");
}

void general_settings::save_settings()
{
    jwrite(VINACOMP_CONF_PATH + "/settings.json", *data);
}

void general_settings::showEvent(QShowEvent *event)
{
    restore_settings();
}

void general_settings::restore_settings()
{
    *data = jread(VINACOMP_CONF_PATH + "/settings.json");

    for (knob *_knob : *knobs)
        _knob->restore_param();

    update_plugin_dirs_tree();
}
