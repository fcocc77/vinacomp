#include <action.h>
#include <knob_editor.h>
#include <qt.h>
#include <tools.h>

knob_editor::knob_editor(QWidget *__properties)
    : _properties(__properties)
    , current_panel(nullptr)
{
    this->setObjectName("knob_editor");

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setMargin(0);
    layout->setSpacing(0);

    temp_widget = new QWidget;
    temp_widget->hide();
    temp_widget->setMinimumHeight(3);
    temp_widget->setStyleSheet(
        "border-bottom: 2px solid green; border-style: dotted;");

    temp_vertical_widget = new QWidget;
    temp_vertical_widget->hide();
    temp_vertical_widget->setMinimumWidth(3);
    temp_vertical_widget->setStyleSheet("border-left: 2px solid green;");

    tools *tools_bar = new tools(20);
    tools_bar->allow_one_check_at_time();
    layout->addWidget(tools_bar);

    // Floating
    action *float_knob_action = new action("Floating Knob", "", "float");
    float_knob_action->set_checkable(true);
    float_knob_action->connect_to(
        this, [=]() { update_edit_options(float_knob_action->is_checked()); });
    tools_bar->add_action(float_knob_action);
    connect(float_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "floating";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Integer
    action *int_knob_action = new action("Integer Knob", "", "int");
    int_knob_action->set_checkable(true);
    int_knob_action->connect_to(
        this, [=]() { update_edit_options(int_knob_action->is_checked()); });
    tools_bar->add_action(int_knob_action);
    connect(int_knob_action->get_button(), &QPushButton::pressed, this, [=]() {
        current_knob_type = "integer";
        this->setCursor(Qt::ClosedHandCursor);
    });
    //
    //

    // Color
    action *color_knob_action = new action("Color Knob", "", "color");
    color_knob_action->set_checkable(true);
    color_knob_action->connect_to(
        this, [=]() { update_edit_options(color_knob_action->is_checked()); });
    tools_bar->add_action(color_knob_action);
    connect(color_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "color";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Button
    action *button_knob_action = new action("Button Knob", "", "button");
    button_knob_action->set_checkable(true);
    button_knob_action->connect_to(
        this, [=]() { update_edit_options(button_knob_action->is_checked()); });
    tools_bar->add_action(button_knob_action);
    connect(button_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "button";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Combo Box
    action *combo_box_knob_action = new action("Choice Knob", "", "combo_box");
    combo_box_knob_action->set_checkable(true);
    combo_box_knob_action->connect_to(this, [=]() {
        update_edit_options(combo_box_knob_action->is_checked());
    });
    tools_bar->add_action(combo_box_knob_action);
    connect(combo_box_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "choice";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // CheckBox
    action *check_box_knob_action =
        new action("CheckBox Knob", "", "check_box");
    check_box_knob_action->set_checkable(true);
    check_box_knob_action->connect_to(this, [=]() {
        update_edit_options(check_box_knob_action->is_checked());
    });
    tools_bar->add_action(check_box_knob_action);
    connect(check_box_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "check_box";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Text
    action *text_knob_action = new action("Text Knob", "", "text");
    text_knob_action->set_checkable(true);
    text_knob_action->connect_to(
        this, [=]() { update_edit_options(text_knob_action->is_checked()); });
    tools_bar->add_action(text_knob_action);
    connect(text_knob_action->get_button(), &QPushButton::pressed, this, [=]() {
        current_knob_type = "text";
        this->setCursor(Qt::ClosedHandCursor);
    });
    //
    //

    // File
    action *file_knob_action = new action("File Knob", "", "create_new_folder");
    file_knob_action->set_checkable(true);
    file_knob_action->connect_to(
        this, [=]() { update_edit_options(file_knob_action->is_checked()); });
    tools_bar->add_action(file_knob_action);
    connect(file_knob_action->get_button(), &QPushButton::pressed, this, [=]() {
        current_knob_type = "file";
        this->setCursor(Qt::ClosedHandCursor);
    });
    //
    //

    // Position
    action *position_knob_action = new action("Position Knob", "", "position");
    position_knob_action->set_checkable(true);
    position_knob_action->connect_to(this, [=]() {
        update_edit_options(position_knob_action->is_checked());
    });
    tools_bar->add_action(position_knob_action);
    connect(position_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "position";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    tools_bar->add_separator();

    // Tab
    action *tab_action = new action("Add Tab", "", "tab");
    tab_action->set_checkable(true);
    tab_action->connect_to(
        this, [=]() { update_edit_options(tab_action->is_checked()); });
    tools_bar->add_action(tab_action);
    connect(tab_action->get_button(), &QPushButton::pressed, this, [=]() {
        current_knob_type = "tab";
        this->setCursor(Qt::ClosedHandCursor);
    });
    //
    //

    // Label
    action *label_knob_action = new action("Label Knob", "", "label");
    label_knob_action->set_checkable(true);
    label_knob_action->connect_to(
        this, [=]() { update_edit_options(label_knob_action->is_checked()); });
    tools_bar->add_action(label_knob_action);
    connect(label_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "label";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Group
    action *group_knob_action = new action("Group Knob", "", "group");
    group_knob_action->set_checkable(true);
    group_knob_action->connect_to(
        this, [=]() { update_edit_options(group_knob_action->is_checked()); });
    tools_bar->add_action(group_knob_action);
    connect(group_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "group";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    // Separator
    action *separator_knob_action =
        new action("Separator Knob", "", "separator");
    separator_knob_action->set_checkable(true);
    separator_knob_action->connect_to(this, [=]() {
        update_edit_options(separator_knob_action->is_checked());
    });
    tools_bar->add_action(separator_knob_action);
    connect(separator_knob_action->get_button(), &QPushButton::pressed, this,
            [=]() {
                current_knob_type = "separator";
                this->setCursor(Qt::ClosedHandCursor);
            });
    //
    //

    tools_bar->add_stretch();

    action *add_action = new action("Add Knob", "", "add");
    add_action->connect_to(this, [=]() { push_knob_or_tab(); });
    tools_bar->add_action(add_action);

    // Caja de edicion
    edit_box = new QWidget(this);
    edit_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    edit_box->setObjectName("edit_box");
    edit_box->hide();
    QVBoxLayout *edit_box_layout = new QVBoxLayout(edit_box);
    edit_box_layout->setAlignment(Qt::AlignTop);
    layout->addWidget(edit_box);

    knob_name = new QLineEdit(this);
    knob_name->setPlaceholderText("Name Knob");

    knob_tips = new QTextEdit(this);
    knob_tips->setMaximumHeight(100);
    knob_tips->setMinimumHeight(100);
    knob_tips->setPlaceholderText("ToolTip Knob");
    //

    // una sola linea
    QWidget *one_line = new QWidget;
    one_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    one_line->setObjectName("one_line");
    QHBoxLayout *one_line_layout = new QHBoxLayout(one_line);
    one_line_layout->setMargin(0);

    minimum_edit = new QLineEdit(this);
    maximum_edit = new QLineEdit(this);
    minimum_edit->setPlaceholderText("Min");
    maximum_edit->setPlaceholderText("Max");

    new_line_check = new knob_check_box("New Line");

    one_line_layout->addWidget(minimum_edit);
    one_line_layout->addWidget(maximum_edit);
    one_line_layout->addWidget(new_line_check);

    edit_box_layout->addWidget(knob_name);
    edit_box_layout->addWidget(one_line);
    edit_box_layout->addWidget(knob_tips);
}

knob_editor::~knob_editor() {}

void knob_editor::update_edit_options(bool visible)
{
    if (current_knob_type == "tab")
        knob_name->setPlaceholderText("Name Tab");
    else
        knob_name->setPlaceholderText("Name Knob");

    // visibilidad de opciones
    knob_tips->hide();
    knob_name->hide();
    minimum_edit->hide();
    maximum_edit->hide();
    new_line_check->hide();

    QStringList list_for_tips{"floating", "integer", "color",
                              "button",   "choice",  "check_box",
                              "text",     "file",    "position"};

    QStringList list_for_name{"floating", "integer",   "color", "button",
                              "choice",   "check_box", "text",  "file",
                              "position", "label",     "group", "tab"};

    QStringList list_for_min_max{"floating", "integer", "color"};
    QStringList list_for_new_line{"floating", "integer", "color"};

    QString type = current_knob_type;

    if (list_for_tips.contains(current_knob_type))
        knob_tips->show();

    if (list_for_name.contains(current_knob_type))
        knob_name->show();

    if (list_for_min_max.contains(current_knob_type))
    {
        minimum_edit->show();
        maximum_edit->show();
    }

    if (list_for_new_line.contains(current_knob_type))
        new_line_check->show();
    //
    //

    edit_box->setVisible(visible);
    this->setCursor(Qt::ArrowCursor);
}
