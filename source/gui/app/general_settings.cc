#include <QSplitter>

#include <general_settings.h>
#include <global.h>
#include <knob_check_box.h>
#include <knob_integer.h>
#include <setup_knobs.h>
#include <util.h>

general_settings::general_settings(QWidget *_vinacomp)
    : settings(true, _vinacomp)
{
    setup_general();
    setup_auto_save();
    setup_plugins();
    setup_appearance();

    set_content("AutoSave");
}

general_settings::~general_settings() {}

void general_settings::setup_general()
{
    QVBoxLayout *layout = add_item("General");
}

void general_settings::setup_auto_save()
{
    QVBoxLayout *layout = add_item("AutoSave");

    setup_knobs_props props;

    QJsonObject enable_knob = {{"type", "check_box"},
                               {"name", "enable_autosave"},
                               {"label", "Enable AutoSave"},
                               {"default", true}};

    QJsonObject delay_knob = {{"type", "integer"},
                              {"name", "delay_autosave"},
                              {"label", "Delay: Seconds"},
                              {"minimum", 1},
                              {"maximum", 30},
                              {"default", 5}};

    props.knobs_array.push_back(enable_knob);
    props.knobs_array.push_back(delay_knob);

    props.layout = layout;
    props.knobs = knobs;

    setup_knobs(props);

    connect(static_cast<knob_check_box *>(get_knob("enable_autosave")),
            &knob_check_box::changed, this,
            [this](bool checked) { data.insert("auto_save_enable", checked); });

    connect(static_cast<knob_integer *>(get_knob("delay_autosave")),
            &knob_integer::changed, this,
            [this](float value) { data.insert("auto_save_delay", value); });
}

void general_settings::setup_plugins()
{
    QVBoxLayout *layout = add_item("Plugins");
}

void general_settings::setup_appearance()
{
    QVBoxLayout *layout = add_item("Appearance");
}

void general_settings::save_settings()
{
    jwrite(VINACOMP_CONF_PATH + "/settings.json", data);
}

void general_settings::showEvent(QShowEvent *event)
{
    restore_settings();
}

QWidget *get_knob(QString name);

void general_settings::restore_settings()
{
    data = jread(VINACOMP_CONF_PATH + "/settings.json");

    static_cast<knob_check_box *>(get_knob("enable_autosave"))
        ->set_check(data.value("auto_save_enable").toBool());

    static_cast<knob_integer *>(get_knob("delay_autosave"))
        ->set_value(data.value("auto_save_delay").toDouble());
}
