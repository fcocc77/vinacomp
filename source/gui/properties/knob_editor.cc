#include <knob_editor.h>
#include <qt.h>
#include <trim_panel.h>

knob_editor::knob_editor(QWidget *__properties)
    : _properties(__properties)
    , insert_knob_or_tab(false)
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
    layout->addWidget(append_tools);

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

    add_knob_action("Floating Knob", "floating");
    add_knob_action("Integer Knob", "integer");
    add_knob_action("Color Knob", "color");
    add_knob_action("Button Knob", "button");
    add_knob_action("Choice Knob", "choice");
    add_knob_action("CheckBox Knob", "check_box");
    add_knob_action("Text Knob", "text");
    add_knob_action("File Knob", "file");
    add_knob_action("Position Knob", "floating_dimensions");

    append_tools->add_separator();

    add_knob_action("Add Tab", "tab");
    add_knob_action("Label Knob", "label");
    add_knob_action("Group Knob", "group");
    add_knob_action("Separator Knob", "separator");

    append_tools->add_stretch();

    // edit mode
    edit_tools = new QWidget(this);
    edit_tools->setObjectName("edit_tools");
    edit_tools->hide();
    QHBoxLayout *edit_tools_layout = new QHBoxLayout(edit_tools);
    edit_tools_layout->setContentsMargins(10, 5, 10, 5);

    QLabel *edit_label_init = new QLabel("Expanding knob: ");
    edit_label = new QLabel("Editando knob 'mi knob' ...");
    edit_label->setObjectName("edit_label");
    edit_icon = new button();
    edit_icon->setObjectName("edit_icon");
    edit_icon->set_icon("color");

    ok_button = new QPushButton("OK");
    cancel_button = new QPushButton("Cancel");

    connect(ok_button, &QPushButton::clicked, this,
            [this]() { finish_edit_knob(true); });

    connect(cancel_button, &QPushButton::clicked, this,
            [this]() { finish_edit_knob(false); });

    edit_tools_layout->addWidget(edit_icon);
    edit_tools_layout->addWidget(edit_label_init);
    edit_tools_layout->addWidget(edit_label);
    edit_tools_layout->addStretch();
    edit_tools_layout->addWidget(cancel_button);
    edit_tools_layout->addWidget(ok_button);


    layout->addWidget(edit_tools);
    //

    add_action = new action("Add Knob", "", "add");
    add_action->set_visible(false);
    add_action->connect_to(this, [=]() { push_knob_or_tab(); });
    append_tools->add_action(add_action);

    // Caja de edicion
    edit_box = new QWidget(this);
    edit_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    edit_box->setObjectName("edit_box");
    edit_box->hide();
    QVBoxLayout *edit_box_layout = new QVBoxLayout(edit_box);
    edit_box_layout->setAlignment(Qt::AlignTop);
    layout->addWidget(edit_box);

    QWidget *name_and_label = new QWidget(this);
    name_and_label->setObjectName("name_and_label");
    QHBoxLayout *name_and_label_layout = new QHBoxLayout(name_and_label);
    name_and_label_layout->setMargin(0);
    knob_name = new QLineEdit(this);
    knob_label = new QLineEdit(this);
    knob_name->setPlaceholderText("Name Knob");
    knob_label->setPlaceholderText("Label Knob");
    knob_name->setToolTip("Name");
    knob_label->setToolTip("Label");
    name_and_label_layout->addWidget(knob_label);
    name_and_label_layout->addWidget(knob_name);

    knob_tips = new QTextEdit(this);
    knob_tips->setMaximumHeight(100);
    knob_tips->setMinimumHeight(100);
    knob_tips->setPlaceholderText("ToolTip Knob");
    knob_tips->setToolTip("ToolTip");
    //

    QWidget *one_line = new QWidget;
    one_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    one_line->setObjectName("one_line");
    QHBoxLayout *one_line_layout = new QHBoxLayout(one_line);
    one_line_layout->setAlignment(Qt::AlignLeft);
    one_line_layout->setMargin(0);

    minimum_edit = new QLineEdit(this);
    maximum_edit = new QLineEdit(this);
    default_value_edit = new QLineEdit(this);
    minimum_edit->setPlaceholderText("Min");
    maximum_edit->setPlaceholderText("Max");
    minimum_edit->setToolTip("Min");
    maximum_edit->setToolTip("Max");
    default_value_edit->setPlaceholderText("Default Value");
    default_value_edit->setToolTip("Default");

    new_line_check = new knob_check_box({}, "New Line");
    bidimensional_check = new knob_check_box({}, "BiDimensional");
    animatable_check = new knob_check_box({}, "Animatable");

    one_line_layout->addWidget(minimum_edit);
    one_line_layout->addWidget(maximum_edit);
    one_line_layout->addWidget(default_value_edit);
    one_line_layout->addWidget(new_line_check);
    one_line_layout->addWidget(bidimensional_check);
    one_line_layout->addWidget(animatable_check);

    edit_box_layout->addWidget(name_and_label);
    edit_box_layout->addWidget(one_line);
    edit_box_layout->addWidget(knob_tips);
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
    new_line_check->hide();
    bidimensional_check->hide();
    animatable_check->hide();
    default_value_edit->hide();

    QStringList list_for_tips{"floating", "integer", "color",
                              "button",   "choice",  "check_box",
                              "text",     "file",    "floating_dimensions"};

    QStringList list_for_name{"floating", "integer", "color",
                              "button",   "choice",  "check_box",
                              "text",     "file",    "floating_dimensions",
                              "label",    "group",   "tab"};

    QStringList list_for_label{"floating", "integer", "color",
                               "button",   "choice",  "check_box",
                               "text",     "file",    "floating_dimensions",
                               "label",    "group"};

    QStringList list_for_min_max{"floating", "integer", "color"};

    QStringList list_for_new_line{"floating", "integer", "color", "choice",
                                  "check_box"};

    QStringList list_for_animatable{"floating", "integer", "color"};

    QStringList list_for_bidimensional{"floating", "integer"};

    QStringList list_for_default_value{"floating", "integer", "color"};

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

    if (list_for_new_line.contains(knob_type))
        new_line_check->show();

    if (list_for_animatable.contains(knob_type))
        animatable_check->show();

    if (list_for_bidimensional.contains(knob_type))
        bidimensional_check->show();

    if (list_for_default_value.contains(knob_type))
        default_value_edit->show();
    //
    //

    edit_box->setVisible(visible);
    this->setCursor(Qt::ArrowCursor);
}

void knob_editor::start_insertion()
{
    insert_index = -2;
    current_panel = nullptr;
    this->setCursor(Qt::ClosedHandCursor);

    insert_knob_or_tab = true;
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

    else if (knob_type == "check_box")
        return "check_box";

    else if (knob_type == "text")
        return "text";

    else if (knob_type == "file")
        return "create_new_folder";

    else if (knob_type == "floating_dimensions")
        return "position";

    else if (knob_type == "tab")
        return "tab";

    else if (knob_type == "label")
        return "label";

    else if (knob_type == "group")
        return "group";

    else if (knob_type == "separator")
        return "separator";
}

void knob_editor::finish_insertion(bool add_item)
{
    if (!insert_knob_or_tab)
        return;

    hide_all_dividing_line();

    this->setCursor(Qt::ArrowCursor);
    if (add_item)
    {
        if (dragging_knob)
            move_knob(current_panel, insert_index);
        else if (current_knob_type == "tab")
            add_tab(current_panel, insert_index);
        else
            add_knob(current_panel, get_params_from_edit_box(current_panel),
                     insert_index);
    }

    for (action *knob_action : actions)
        knob_action->set_illuminated_button(false);

    // deja en preferencia el tipo de dato actual, al boton checkeado una vez se
    // utilizo el drag
    current_knob_type = checked_knob_type;

    insert_knob_or_tab = false;
    dragging_knob = nullptr;
}

void knob_editor::dragging_insertion(QPointF pos)
{
    if (!insert_knob_or_tab)
        return;

    if (current_knob_type == "tab")
        insert_division_to_tabs(pos);
    else
        insert_division_to_knobs();
}

void knob_editor::mouseMoveEvent(QMouseEvent *event)
{
    dragging_insertion(event->pos());
}

void knob_editor::mouseReleaseEvent(QMouseEvent *event)
{
    finish_insertion();
}

