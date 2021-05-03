#include <knob_editor.h>
#include <action.h>
#include <tools.h>
#include <qt.h>

knob_editor::knob_editor(QWidget *_panel)
    : panel(_panel)
{
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setMargin(0);

    temp_widget = new QWidget;
    temp_widget->hide();
    temp_widget->setMinimumHeight(3);
    temp_widget->setStyleSheet(
        "border-bottom: 2px solid green; border-style: dotted;");

    tools *tools_bar = new tools(20);
    tools_bar->allow_one_check_at_time();
    layout->addWidget(tools_bar);

    // Floating
    action *float_knob_action = new action("Floating Knob", "", "float");
    float_knob_action->set_checkable(true);
    float_knob_action->connect_to(this, [=]() {
        minimum_edit->show();
        maximum_edit->show();
        edit_box->setVisible(float_knob_action->is_checked());
        current_knob_type = "floating";
    });
    //
    //

    // Integer
    action *int_knob_action = new action("Integer Knob", "", "int");
    int_knob_action->set_checkable(true);
    int_knob_action->connect_to(this, [=]() {
        minimum_edit->show();
        maximum_edit->show();
        edit_box->setVisible(int_knob_action->is_checked());
        current_knob_type = "integer";
    });
    //
    //

    // Color
    action *color_knob_action = new action("Color Knob", "", "color");
    color_knob_action->set_checkable(true);
    color_knob_action->connect_to(this, [=]() {
        minimum_edit->show();
        maximum_edit->show();
        edit_box->setVisible(color_knob_action->is_checked());
        current_knob_type = "color";
    });
    //
    //

    // Button
    action *button_knob_action = new action("Button Knob", "", "button");
    button_knob_action->set_checkable(true);
    button_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(button_knob_action->is_checked());
        current_knob_type = "button";
    });
    //
    //

    // Combo Box
    action *combo_box_knob_action = new action("Choice Knob", "", "combo_box");
    combo_box_knob_action->set_checkable(true);
    combo_box_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(combo_box_knob_action->is_checked());
        current_knob_type = "choice";
    });
    //
    //

    // CheckBox
    action *check_box_knob_action = new action("CheckBox Knob", "", "check_box");
    check_box_knob_action->set_checkable(true);
    check_box_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(check_box_knob_action->is_checked());
        current_knob_type = "check_box";
    });
    //
    //

    // Text
    action *text_knob_action = new action("Text Knob", "", "text");
    text_knob_action->set_checkable(true);
    text_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(text_knob_action->is_checked());
        current_knob_type = "text";
    });
    //
    //

    // File
    action *file_knob_action = new action("File Knob", "", "create_new_folder");
    file_knob_action->set_checkable(true);
    file_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(file_knob_action->is_checked());
        current_knob_type = "file";
    });
    //
    //

    // Position
    action *position_knob_action = new action("Position Knob", "", "position");
    position_knob_action->set_checkable(true);
    position_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(position_knob_action->is_checked());
        current_knob_type = "floating_dimensions";
    });
    //
    //

    // Label
    action *label_knob_action = new action("Label Knob", "", "label");
    label_knob_action->set_checkable(true);
    label_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(label_knob_action->is_checked());
        current_knob_type = "label";
    });
    //
    //

    // Group
    action *group_knob_action = new action("Group Knob", "", "group");
    group_knob_action->set_checkable(true);
    group_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(group_knob_action->is_checked());
        current_knob_type = "group";
    });
    //
    //

    // Separator
    action *separator_knob_action = new action("Separator Knob", "", "separator");
    separator_knob_action->set_checkable(true);
    separator_knob_action->connect_to(this, [=]() {
        minimum_edit->hide();
        maximum_edit->hide();
        edit_box->setVisible(separator_knob_action->is_checked());
        current_knob_type = "separator";
    });
    //
    //

    tools_bar->add_action(float_knob_action);
    tools_bar->add_action(int_knob_action);
    tools_bar->add_action(color_knob_action);
    tools_bar->add_action(button_knob_action);
    tools_bar->add_action(combo_box_knob_action);
    tools_bar->add_action(check_box_knob_action);
    tools_bar->add_action(text_knob_action);
    tools_bar->add_action(file_knob_action);
    tools_bar->add_action(position_knob_action);
    tools_bar->add_separator();
    tools_bar->add_action(label_knob_action);
    tools_bar->add_action(group_knob_action);
    tools_bar->add_action(separator_knob_action);
    tools_bar->add_stretch();

    // Caja de edicion
    edit_box = new QWidget(this);
    edit_box->hide();
    QHBoxLayout *edit_box_layout = new QHBoxLayout(edit_box);
    layout->addWidget(edit_box);

    knob_name = new QLineEdit(this);
    knob_label = new QLineEdit(this);
    minimum_edit = new QLineEdit(this);
    maximum_edit = new QLineEdit(this);
    new_line_label = new QLabel("New Line");
    new_line_check = new QCheckBox(this);

    minimum_edit->setMaximumWidth(70);
    maximum_edit->setMaximumWidth(70);

    knob_name->setPlaceholderText("Name Knob");
    knob_label->setPlaceholderText("Label Knob");
    minimum_edit->setPlaceholderText("Min: 0");
    maximum_edit->setPlaceholderText("Max: 100");

    QPushButton *drag_button = new QPushButton(this);
    qt::set_icon(drag_button, "position_a", 20);

    QPushButton *add_button = new QPushButton(this);
    connect(add_button, &QPushButton::clicked, this, &knob_editor::add_knob);
    qt::set_icon(add_button, "add_a", 20);

    edit_box_layout->addWidget(knob_name);
    edit_box_layout->addWidget(knob_label);
    edit_box_layout->addWidget(new_line_check);
    edit_box_layout->addWidget(new_line_label);
    edit_box_layout->addWidget(minimum_edit);
    edit_box_layout->addWidget(maximum_edit);
    edit_box_layout->addWidget(drag_button);
    edit_box_layout->addWidget(add_button);
}

knob_editor::~knob_editor() {}
