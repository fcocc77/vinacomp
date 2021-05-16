#include <QSplitter>

#include <general_settings.h>
#include <knob_check_box.h>
#include <knob_integer.h>

general_settings::general_settings()
    : settings(true)
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

    enable_knob->set_init_space(init_space);
    delay_knob->set_init_space(init_space);
    delay_knob->set_init_label_text("Delay: Seconds");

    layout->addWidget(enable_knob);
    layout->addWidget(delay_knob);
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
    print("save");
}
