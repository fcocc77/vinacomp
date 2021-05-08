#include <knob_editor.h>
#include <qt.h>
#include <tools.h>
#include <trim_panel.h>

knob_editor::knob_editor(QWidget *__properties)
    : _properties(__properties)
    , insert_knob_or_tab(false)
    , dragging_knob(nullptr)
    , current_panel(nullptr)
{
    this->setObjectName("knob_editor");

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setMargin(0);
    layout->setSpacing(0);

    tools *tools_bar = new tools(20);
    tools_bar->allow_one_check_at_time();
    layout->addWidget(tools_bar);

    auto add_knob_action = [=](QString label, QString icon, QString id) {
        action *knob_action = new action(label, "", icon);
        knob_action->set_checkable(true);
        knob_action->connect_to(this, [=]() {
            bool checked = knob_action->is_checked();
            update_edit_options(checked);

            knob_action->set_illuminated_button(false);
            checked_knob_type = id;

            add_action->set_visible(checked);

            if (!checked)
                current_knob_type = "";
        });
        tools_bar->add_action(knob_action);
        connect(knob_action->get_button(), &QPushButton::pressed, this, [=]() {
            knob_action->set_illuminated_button(true);
            current_knob_type = id;
            start_insertion();
        });
        actions.push_back(knob_action);
    };

    add_knob_action("Floating Knob", "float", "floating");
    add_knob_action("Integer Knob", "int", "integer");
    add_knob_action("Color Knob", "color", "color");
    add_knob_action("Button Knob", "button", "button");
    add_knob_action("Choice Knob", "combo_box", "choice");
    add_knob_action("CheckBox Knob", "check_box", "check_box");
    add_knob_action("Text Knob", "text", "text");
    add_knob_action("File Knob", "create_new_folder", "file");
    add_knob_action("Position Knob", "position", "floating_dimensions");

    tools_bar->add_separator();

    add_knob_action("Add Tab", "tab", "tab");
    add_knob_action("Label Knob", "label", "label");
    add_knob_action("Group Knob", "group", "group");
    add_knob_action("Separator Knob", "separator", "separator");

    tools_bar->add_stretch();

    add_action = new action("Add Knob", "", "add");
    add_action->set_visible(false);
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
    default_value_edit = new QLineEdit(this);
    minimum_edit->setPlaceholderText("Min");
    maximum_edit->setPlaceholderText("Max");
    default_value_edit->setPlaceholderText("Default Value");

    new_line_check = new knob_check_box({}, "New Line");
    bidimensional_check = new knob_check_box({}, "BiDimensional");
    animatable_check = new knob_check_box({}, "Animatable");

    one_line_layout->addWidget(minimum_edit);
    one_line_layout->addWidget(maximum_edit);
    one_line_layout->addWidget(default_value_edit);
    one_line_layout->addWidget(new_line_check);
    one_line_layout->addWidget(bidimensional_check);
    one_line_layout->addWidget(animatable_check);

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

    QStringList list_for_min_max{"floating", "integer", "color"};
    QStringList list_for_new_line{"floating", "integer", "color"};
    QStringList list_for_animatable{"floating", "integer", "color"};
    QStringList list_for_bidimensional{"floating", "integer"};
    QStringList list_for_default_value{"floating", "integer", "color"};

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

    if (list_for_animatable.contains(current_knob_type))
        animatable_check->show();

    if (list_for_bidimensional.contains(current_knob_type))
        bidimensional_check->show();

    if (list_for_default_value.contains(current_knob_type))
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

