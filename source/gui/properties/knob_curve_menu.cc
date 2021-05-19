#include <action.h>
#include <knob_curve_menu.h>
#include <script_editor.h>
#include <trim_panel.h>
#include <util.h>

knob_curve_menu::knob_curve_menu(QWidget *_panel)
    : QMenu(_panel)
    , current_knob(0)
    , panel(_panel)
{
    action *set_key_action = new action("Set Key", "", "key");
    action *delete_key_action = new action("Delete Key", "", "close");
    action *no_animation_action = new action("No Animation", "");
    action *reset_to_default_action =
        new action("Reset to default", "", "restart");

    action *curve_editor_action =
        new action("Curve Editor...", "", "curve_editor");
    action *edit_expression_action = new action("Edit Expression", "", "code");
    action *clear_expression_action =
        new action("Clear Expression", "", "close");

    action *copy_values_action = new action("Copy Values", "");
    action *copy_animation_action = new action("Copy Animation", "");
    action *copy_links_action = new action("Copy Links", "", "link");

    action *paste_action = new action("Paste", "", "paste");

    action *generate_keys_action =
        new action("Generate Keys", "", "generate_keys");
    action *smooth_curve_action =
        new action("Smooth Curve", "", "curve_smooth");
    action *loop_action = new action("Loop", "", "refresh");

    this->addAction(set_key_action);
    this->addAction(delete_key_action);
    this->addAction(no_animation_action);
    this->addAction(reset_to_default_action);
    this->addSeparator();
    this->addAction(curve_editor_action);
    this->addAction(edit_expression_action);
    this->addAction(clear_expression_action);
    this->addSeparator();
    this->addAction(copy_values_action);
    this->addAction(copy_animation_action);
    this->addAction(copy_links_action);
    this->addSeparator();
    this->addAction(paste_action);
    this->addSeparator();
    this->addAction(generate_keys_action);
    this->addAction(smooth_curve_action);
    this->addAction(loop_action);

    // Conecciones
    set_key_action->connect_to(panel, [this]() { set_key(); });

    delete_key_action->connect_to(panel, [=]() {});

    no_animation_action->connect_to(panel, [this]() { no_animation(); });

    reset_to_default_action->connect_to(panel, [this]() { reset_default(); });

    edit_expression_action->connect_to(panel, [this]() { edit_expression(); });

    clear_expression_action->connect_to(panel,
                                        [this]() { clear_expression(); });

    curve_editor_action->connect_to(panel, [=]() {});

    copy_values_action->connect_to(panel, [this]() { copy("values"); });

    copy_animation_action->connect_to(panel, [this]() { copy("animation"); });

    copy_links_action->connect_to(panel, [this]() { copy("link"); });

    paste_action->connect_to(panel, [this]() { paste(); });

    generate_keys_action->connect_to(panel, [=]() {});
    smooth_curve_action->connect_to(panel, [=]() {});
    loop_action->connect_to(panel, [=]() {});
}

knob_curve_menu::~knob_curve_menu() {}

void knob_curve_menu::set_key()
{
    current_knob->enable_animation();
    current_knob->set_animated(true);
}

void knob_curve_menu::no_animation()
{
    current_knob->disable_animation();
    current_knob->set_animated(false);
}

void knob_curve_menu::edit_expression()
{
    script_editor *expression_editor = static_cast<trim_panel *>(panel)
                                           ->get_properties()
                                           ->get_expression_editor();

    expression_editor->set_knob(current_knob);
}
void knob_curve_menu::clear_expression()
{
    current_knob->set_expression("");
}

void knob_curve_menu::reset_default() {}

void knob_curve_menu::copy(QString copy_action)
{
    static_cast<trim_panel *>(panel)->get_properties()->set_copied_knob_value(
        {current_knob, copy_action});
}

void knob_curve_menu::paste()
{
    auto copied_knob_value = static_cast<trim_panel *>(panel)
                           ->get_properties()
                           ->get_copied_knob_value();

    knob *copied_knob = copied_knob_value.first;
    QString copied_action = copied_knob_value.second;

    if (copied_action == "link")
        current_knob->set_link(copied_knob);
}
