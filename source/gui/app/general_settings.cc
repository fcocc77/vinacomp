#include <QSplitter>

#include <general_settings.h>
#include <global.h>
#include <knob_check_box.h>
#include <knob_integer.h>
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
    int init_space = 100;

    knob_check_box *enable_knob =
        new knob_check_box({}, "Enable AutoSave", true);
    knob_integer *delay_knob = new knob_integer({}, 1, 30, 5);

    connect(enable_knob, &knob_check_box::changed, this,
            [this](bool checked) { data.insert("auto_save_enable", checked); });

    connect(delay_knob, &knob_integer::changed, this,
            [this](float value) { data.insert("auto_save_delay", value); });

    enable_knob->set_init_space(init_space);
    delay_knob->set_init_space(init_space);
    delay_knob->set_init_label_text("Delay: Seconds");

    layout->addWidget(enable_knob);
    layout->addWidget(delay_knob);

    auto_save_enable_pointer = enable_knob;
    auto_save_delay_pointer = delay_knob;
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

void general_settings::restore_settings()
{
    data = jread(VINACOMP_CONF_PATH + "/settings.json");

    static_cast<knob_check_box *>(auto_save_enable_pointer)
        ->set_check(data.value("auto_save_enable").toBool());

    static_cast<knob_integer *>(auto_save_delay_pointer)
        ->set_value(data.value("auto_save_delay").toDouble());
}
