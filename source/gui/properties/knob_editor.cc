#include <knob_editor.h>
#include <qt.h>
#include <trim_panel.h>
#include <knob_floatd.h>

knob_editor::knob_editor(QWidget *__properties)
    : _properties(__properties)
    , insert_knob_or_tab(false)
    , editing(false)
    , last_knob_under_cursor(nullptr)
    , dragging_knob(nullptr)
    , editing_knob(nullptr)
    , current_panel(nullptr)
{
    this->setObjectName("knob_editor");

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setMargin(0);
    layout->setSpacing(0);

    append_tools = new tools(20);
    append_tools->allow_one_check_at_time();

    auto add_knob_action = [=](QString label, QString id) {
        action *knob_action =
            new action(label, "", get_icon_name_from_type(id));
        knob_action->set_checkable(true);
        knob_action->connect_to(this, [=]() {
            bool checked = knob_action->is_checked();
            update_edit_options_from_type(checked, current_knob_type);

            knob_action->set_illuminated_button(false);
            checked_knob_type = id;

            add_action->set_visible(checked);

            if (!checked)
                current_knob_type = "";
        });
        append_tools->add_action(knob_action);
        connect(knob_action->get_button(), &QPushButton::pressed, this, [=]() {
            knob_action->set_illuminated_button(true);
            current_knob_type = id;
            start_insertion();
        });
        actions.push_back(knob_action);
    };

    // News
    edit_tools = new QWidget(this);
    QHBoxLayout *edit_tools_layout = new QHBoxLayout(edit_tools);

    QLabel *edit_label_init = new QLabel("Modifying knob: ");
    edit_label = new QLabel();
    edit_icon = new button();

    ok_button = new QPushButton("OK");
    cancel_button = new QPushButton("Cancel");

    add_action = new action("Add Knob", "", "add");

    edit_box = new QWidget(this);
    QVBoxLayout *edit_box_layout = new QVBoxLayout(edit_box);

    QWidget *name_and_label = new QWidget(this);
    QHBoxLayout *name_and_label_layout = new QHBoxLayout(name_and_label);
    knob_name = new QLineEdit(this);
    knob_label = new QLineEdit(this);

    knob_tips = new QTextEdit(this);

    QWidget *one_line = new QWidget;
    QHBoxLayout *one_line_layout = new QHBoxLayout(one_line);

    minimum_edit = new QLineEdit(this);
    maximum_edit = new QLineEdit(this);
    default_value_edit = new QLineEdit(this);
    over_line_check = new knob_check_box({}, "Over Line");
    bidimensional_check = new knob_check_box({}, "BiDimensional");
    allowed_file_types = new QTextEdit(this);
    choice_items_edit = new QTextEdit(this);
    save_file_dialog_check = new knob_check_box({}, "Save File Dialog");
    dir_mode_check = new knob_check_box({}, "Dir Mode");
    image_sequence_check = new knob_check_box({}, "Allow Image Sequence");

    // Options
    {
        edit_tools->setObjectName("edit_tools");
        edit_tools->hide();
        edit_tools_layout->setContentsMargins(10, 5, 10, 5);

        edit_label->setObjectName("edit_label");
        edit_icon->setObjectName("edit_icon");
        edit_icon->set_icon("color");

        add_action->set_visible(false);
        add_action->connect_to(this, [=]() { push_knob_or_tab(); });

        edit_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        edit_box->setObjectName("edit_box");
        edit_box->hide();
        edit_box_layout->setAlignment(Qt::AlignTop);

        name_and_label->setObjectName("name_and_label");
        name_and_label_layout->setMargin(0);

        knob_name->setPlaceholderText("Name Knob");
        knob_name->setToolTip("Name");

        knob_label->setPlaceholderText("Label Knob");
        knob_label->setToolTip("Label");

        knob_tips->setFixedHeight(100);
        knob_tips->setPlaceholderText("ToolTip Knob");
        knob_tips->setToolTip("ToolTip");

        one_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        one_line->setObjectName("one_line");

        one_line_layout->setAlignment(Qt::AlignLeft);
        one_line_layout->setMargin(0);

        minimum_edit->setPlaceholderText("Min");
        maximum_edit->setPlaceholderText("Max");
        minimum_edit->setToolTip("Min");
        maximum_edit->setToolTip("Max");

        default_value_edit->setPlaceholderText("Default Value");
        default_value_edit->setToolTip("Default");

        over_line_check->setToolTip(
            "The 'over line' can only be placed next to a \n"
            "check_box, button, choice or position type, if it is placed \n"
            "next to another type, it will be disabled");

        allowed_file_types->setFixedHeight(70);
        allowed_file_types->setPlaceholderText("Allowed File Type List");
        allowed_file_types->setToolTip(
            "Each file type must be separated by a line break");

        choice_items_edit->setFixedHeight(70);
        choice_items_edit->setPlaceholderText("Item List");
        choice_items_edit->setToolTip(
            "Each element must be separated by a line break, each line must "
            "have 2 "
            "\n"
            "values separated by the '&' sign Ej: 'Titulo & value_name', the "
            "first "
            "value is the label and the \n"
            "second is the name, if you only put one value, the name and the \n"
            "label will remain with that value");
    }

    // Conecciones
    {
        connect(ok_button, &QPushButton::clicked, this,
                [this]() { finish_edit(true); });

        connect(cancel_button, &QPushButton::clicked, this,
                [this]() { finish_edit(false); });
    }

    // Layout
    {
        add_knob_action("Floating Knob", "floating");
        add_knob_action("Integer Knob", "integer");
        add_knob_action("Color Knob", "color");
        add_knob_action("Button Knob", "button");
        add_knob_action("Choice Knob", "choice");
        add_knob_action("CheckBox Knob", "check_box");
        add_knob_action("Text Knob", "text");
        add_knob_action("File Knob", "file");
        add_knob_action("2D Position Knob", "2d_position");
        add_knob_action("3D Position Knob", "3d_position");

        append_tools->add_separator();

        add_knob_action("Add Tab", "tab");
        add_knob_action("Group Knob", "group");
        add_knob_action("Label Knob", "label");
        add_knob_action("Separator Knob", "separator");

        append_tools->add_stretch();
        append_tools->add_action(add_action);

        edit_tools_layout->addWidget(edit_icon);
        edit_tools_layout->addWidget(edit_label_init);
        edit_tools_layout->addWidget(edit_label);
        edit_tools_layout->addStretch();
        edit_tools_layout->addWidget(cancel_button);
        edit_tools_layout->addWidget(ok_button);

        name_and_label_layout->addWidget(knob_label);
        name_and_label_layout->addWidget(knob_name);

        one_line_layout->addWidget(minimum_edit);
        one_line_layout->addWidget(maximum_edit);
        one_line_layout->addWidget(default_value_edit);
        one_line_layout->addWidget(choice_items_edit);
        one_line_layout->addWidget(over_line_check);
        one_line_layout->addWidget(bidimensional_check);
        one_line_layout->addWidget(allowed_file_types);
        one_line_layout->addWidget(save_file_dialog_check);
        one_line_layout->addWidget(dir_mode_check);
        one_line_layout->addWidget(image_sequence_check);

        edit_box_layout->addWidget(name_and_label);
        edit_box_layout->addWidget(one_line);
        edit_box_layout->addWidget(knob_tips);

        layout->addWidget(append_tools);
        layout->addWidget(edit_tools);
        layout->addWidget(edit_box);
    }
}

knob_editor::~knob_editor() {}

void knob_editor::update_edit_options_from_type(bool visible, QString knob_type)
{
    if (knob_type == "tab")
        knob_name->setPlaceholderText("Name Tab");
    else
        knob_name->setPlaceholderText("Name Knob");

    // visibilidad de opciones
    knob_tips->hide();
    knob_name->hide();
    knob_label->hide();
    minimum_edit->hide();
    maximum_edit->hide();
    over_line_check->hide();
    bidimensional_check->hide();
    default_value_edit->hide();
    allowed_file_types->hide();
    choice_items_edit->hide();
    save_file_dialog_check->hide();
    dir_mode_check->hide();
    image_sequence_check->hide();

    QStringList list_for_tips{"floating",    "integer",    "color", "button",
                              "choice",      "check_box",  "text",  "file",
                              "2d_position", "3d_position"};

    QStringList list_for_name{"floating",    "integer",     "color", "button",
                              "choice",      "check_box",   "text",  "file",
                              "2d_position", "3d_position", "label", "group",
                              "tab",         "separator"};

    QStringList list_for_label{"floating",    "integer",     "color", "button",
                               "choice",      "check_box",   "text",  "file",
                               "2d_position", "3d_position", "label", "group"};

    QStringList list_for_min_max{"floating", "integer", "color"};

    QStringList list_for_over_line{"choice", "check_box", "button",
                                   "2d_position", "3d_position"};

    QStringList list_for_bidimensional{"floating", "integer"};

    QStringList list_for_default_value{"floating", "integer", "color"};

    QStringList list_for_choice_items{"choice"};


    if (list_for_tips.contains(knob_type))
        knob_tips->show();

    if (list_for_name.contains(knob_type))
        knob_name->show();

    if (list_for_label.contains(knob_type))
        knob_label->show();

    if (list_for_min_max.contains(knob_type))
    {
        minimum_edit->show();
        maximum_edit->show();
    }

    if (list_for_over_line.contains(knob_type))
        over_line_check->show();

    if (list_for_bidimensional.contains(knob_type))
        bidimensional_check->show();

    if (list_for_default_value.contains(knob_type))
        default_value_edit->show();

    if (list_for_choice_items.contains(knob_type))
        choice_items_edit->show();

    if (knob_type == "file")
    {
        save_file_dialog_check->show();
        allowed_file_types->show();
        dir_mode_check->show();
        image_sequence_check->show();
    }

    edit_box->setVisible(visible);
    this->setCursor(Qt::ArrowCursor);
}

QString knob_editor::get_icon_name_from_type(QString knob_type) const
{
    if (knob_type == "floating")
        return "float";

    else if (knob_type == "integer")
        return "int";

    else if (knob_type == "color")
        return "color";

    else if (knob_type == "button")
        return "button";

    else if (knob_type == "choice")
        return "combo_box";

    else if (knob_type == "check_box")
        return "check_box";

    else if (knob_type == "text")
        return "text";

    else if (knob_type == "file")
        return "create_new_folder";

    else if (knob_type == "2d_position")
        return "position";

    else if (knob_type == "3d_position")
        return "3d";

    else if (knob_type == "tab")
        return "tab";

    else if (knob_type == "label")
        return "label";

    else if (knob_type == "group")
        return "group";

    else if (knob_type == "separator")
        return "separator";
}

QString knob_editor::get_type_from_knob(knob *_knob) const
{
    knob_dimensional *knobd = dynamic_cast<knob_dimensional *>(_knob);

    if (!knobd)
        return _knob->get_type();

    int dimensions = knobd->get_dimensions_count();

    if (dimensions == 2)
        return "2d_position";
    else if (dimensions == 3)
        return "3d_position";
}

void knob_editor::mouseMoveEvent(QMouseEvent *event)
{
    dragging_insertion(event->pos());
}

void knob_editor::mouseReleaseEvent(QMouseEvent *event)
{
    finish_insertion();
}
