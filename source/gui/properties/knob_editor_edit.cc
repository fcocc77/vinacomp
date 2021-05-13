#include <knob_editor.h>
#include <trim_panel.h>
#include <util.h>
#include <properties.h>
#include <tab_widget.h>

void knob_editor::push_knob_or_tab()
{
    // inserta el knob o tab en el primer panel, si es que hay uno
    QWidget *panel = static_cast<properties *>(_properties)->get_first_panel();
    if (!panel)
        return;

    if (current_knob_type == "tab")
    {
        add_tab(panel);
        return;
    }

    add_knob(panel, get_params_from_edit_box(panel));
}

void knob_editor::add_knob(QWidget *panel, knob_params params, int index)
{
    if (index == -2 || !panel)
        return;

    trim_panel *_panel = static_cast<trim_panel *>(panel);

    QString custom_tab = get_custom_tab_name(panel);

    QJsonObject knob_object;
    if (params.type == "floating")
    {
        knob_object = {{"name", params.name},
                       {"type", "floating"},
                       {"label", params.label},
                       {"tooltip", params.tips},
                       {"minimum", params.min},
                       {"maximum", params.max},
                       {"default", params.default_value},
                       {"bidimensional", params.bidimensional},
                       {"tab", custom_tab}};
    }
    else if (params.type == "integer")
    {
        knob_object = {{"name", params.name},
                       {"type", "integer"},
                       {"label", params.label},
                       {"tooltip", params.tips},
                       {"minimum", params.min},
                       {"maximum", params.max},
                       {"bidimensional", params.bidimensional},
                       {"default", params.default_value},
                       {"tab", custom_tab}};
    }
    else if (params.type == "color")
    {
        float def = params.default_value;

        knob_object = {{"name", params.name},
                       {"type", "color"},
                       {"label", params.label},
                       {"tooltip", params.tips},
                       {"minimum", params.min},
                       {"maximum", params.max},
                       {"centered_handler", true},
                       {"tab", custom_tab},
                       {"default", QJsonArray{def, def, def, def}}};
    }
    else if (params.type == "button")
    {
        knob_object = {{"name", params.name},
                       {"type", "button"},
                       {"label", params.label},
                       {"tab", custom_tab},
                       {"over_line", params.over_line},
                       {"tooltip", params.tips}};
    }
    else if (params.type == "choice")
    {
        knob_object = {{"name", params.name},         {"type", "choice"},
                       {"label", params.label},       {"tooltip", params.tips},
                       {"tab", custom_tab},           {"items", QJsonArray{}},
                       {"over_line", params.over_line},
                       {"default", QJsonArray{0, ""}}};
    }
    else if (params.type == "check_box")
    {
        knob_object = {{"name", params.name},   {"type", "check_box"},
                       {"label", params.label}, {"tooltip", params.tips},
                       {"default", false},      {"over_line", params.over_line},
                       {"tab", custom_tab}};
    }
    else if (params.type == "text")
    {
        knob_object = {{"name", params.name},   {"type", "text"},
                       {"label", params.label}, {"tooltip", params.tips},
                       {"default", ""},         {"tab", custom_tab}};
    }
    else if (params.type == "file")
    {

        knob_object = {{"name", params.name},
                       {"type", "file"},
                       {"save_file_dialog", params.save_file_dialog},
                       {"allowed_file_types", params.allowed_file_types},
                       {"label", params.label},
                       {"tooltip", params.tips},
                       {"default", ""},
                       {"tab", custom_tab}};
    }
    else if (params.type == "floating_dimensions")
    {
        knob_object = {{"name", params.name},
                       {"type", "floating_dimensions"},
                       {"label", params.label},
                       {"tooltip", params.tips},
                       {"dimensions", 2},
                       {"over_line", params.over_line},
                       {"default", QJsonArray{0, 0}},
                       {"tab", custom_tab}};
    }
    else if (params.type == "label")
    {
        knob_object = {{"name", params.name},
                       {"type", "label"},
                       {"label", params.label},
                       {"tab", custom_tab}};
    }
    else if (params.type == "group")
    {
        knob_object = {{"name", params.name},   {"type", "group"},
                       {"label", params.label}, {"open", false},
                       {"knobs", 2},     {"tab", custom_tab}};
    }
    else if (params.type == "separator")
    {
        knob_object = {
            {"name", params.name}, {"type", "separator"}, {"tab", custom_tab}};
    }

    if (knob_object.empty())
        return;

    QJsonArray *knobs = _panel->custom_knobs;

    if (index == -1)
        insert_knob_in_tab(knobs, knob_object, custom_tab);
    else
    {
        if (knobs->count() < index)
            return;
        insert_knob_in_tab(knobs, knob_object, custom_tab, index);
    }

    _panel->update_custom_knobs();
    _panel->set_edit_mode(true);
}

void knob_editor::move_knob(QWidget *panel, int index)
{
    if (!panel)
        return;

    // cancela la edicion si un knob se esta editando
    if (editing_knob)
        finish_edit(false);

    QJsonObject knob_data = dragging_knob->get_knob_data();

    knob_params params;
    QString name = knob_data.value("name").toString();

    params.type = knob_data.value("type").toString();
    params.tips = knob_data.value("tooltip").toString();
    params.label = knob_data.value("label").toString();
    params.min = knob_data.value("minimum").toDouble();
    params.max = knob_data.value("maximum").toDouble();
    params.default_value = knob_data.value("default").toDouble();
    params.bidimensional = knob_data.value("bidimensional").toBool();

    // cualquier knob que se mueva queda sin 'over_line'
    params.over_line = false;

    int dragging_knob_index =
        get_index_knob(panel, dragging_knob->get_name());

    QString next_knob_name = disable_over_line_to_next_knob(panel, name);

    // resta uno al index cuando el index es mayor al del knob arrastrado, por
    // que el knob se elimina antes de añadir uno nuevo
    if (!next_knob_name.isEmpty())
    {
        if (dragging_knob_index < index - 1)
            index--;
    }
    else
    {
        if (dragging_knob_index < index)
            index--;
    }

    update_knob(dragging_knob, params, index, true);
}

void knob_editor::update_knob(knob *_knob, knob_params params, int index,
                              bool keep_name)
{
    trim_panel *_panel = static_cast<trim_panel *>(_knob->get_panel());
    QString name = _knob->get_name();

    if (keep_name)
        params.name = name;

    // rescata el parametro antes de borrar el knob, ya que el 'delete_knob'
    // borra el parametro
    QJsonValue param_value = _panel->get_params()->value(name);
    QJsonValue param_curve = _panel->get_params()->value(name + "_curve");
    QJsonValue param_exp = _panel->get_params()->value(name + "_exp");

    delete_knob(_knob);

    // resaura los valores del parametro en el proyecto, si es que existe
    if (!param_value.isUndefined())
        _panel->get_params()->insert(params.name, param_value);
    if (!param_curve.isUndefined())
        _panel->get_params()->insert(params.name + "_curve", param_curve);
    if (!param_exp.isUndefined())
        _panel->get_params()->insert(params.name + "_exp", param_exp);

    add_knob(_panel, params, index);
}

QString knob_editor::disable_over_line_to_next_knob(QWidget *panel,
                                                    QString knob_name)
{
    // si es que el 'knob' esta dentro de un 'line_widget' busca el hermano
    // siguiente y ya que al mover este, el siguiente quedara ubicado primero en
    // el 'line_widget' se tiene que desabilitar el 'over_line'
    QString next_knob_name;
    auto brother_knobs = get_line_widget_knobs(panel, knob_name);
    // bool is_line_widget = !brother_knobs.empty();
    if (brother_knobs.count() >= 2)
        next_knob_name = brother_knobs.value(1)->get_name();

    trim_panel *_panel = static_cast<trim_panel *>(panel);
    for (int i = 0; i < _panel->custom_knobs->count(); i++)
    {
        QJsonObject __knob = (*_panel->custom_knobs)[i].toObject();
        QString __knob_name = __knob.value("name").toString();
        if (__knob_name == next_knob_name)
        {
            __knob["over_line"] = false;
            (*_panel->custom_knobs)[i] = __knob;
        }
    }
    //

    return next_knob_name;
}

void knob_editor::insert_knob_in_tab(QJsonArray *knobs, QJsonObject knob_obj,
                                     QString tab_name, int index)
{
    // inserta el knob en el index correcto, ya que todos los 'custom_knobs' van
    // a estar en una misma lista. Separando los knobs en 2 listas una del tab y
    // otra con los demas knobs, se inserta el knob a la lista de tab y luego se
    // unen las 2 listas.
    QJsonArray knobs_other_tabs;
    auto knobs_from_tab = get_knobs_by_tab(*knobs, tab_name, &knobs_other_tabs);

    // evita que el nuevo knob quede sobre otro knob que no es permitido que
    // este en una linea
    auto previous_knob_line = knobs_from_tab.value(index - 1);
    if (!previous_knob_line.empty())
    {
        QStringList allowed_over_line = {"check_box", "choice",
                                         "floating_dimensions", "button"};

        QString prev_type = previous_knob_line.last().value("type").toString();
        for (QString allowed : allowed_over_line)
            if (!allowed_over_line.contains(prev_type))
                knob_obj["over_line"] = false;
    }
    //
    //

    if (index == -1)
        knobs_from_tab.push_back({knob_obj});
    else
        knobs_from_tab.insert(index, {knob_obj});

    // union de knobs
    *knobs = knobs_other_tabs;
    for (auto list_knobs : knobs_from_tab)
        for (QJsonObject __knob : list_knobs)
            knobs->push_back(__knob);
}

QString knob_editor::add_tab(QWidget *panel, int index, QString preferred_name)
{
    trim_panel *_panel = static_cast<trim_panel *>(panel);
    if (!_panel)
        return "";

    QString tab_name = get_available_tab_name(panel, preferred_name);

    _panel->add_tab(tab_name, index);
    _panel->set_edit_mode(true);

    return tab_name;
}

void knob_editor::edit_box_clear()
{
    knob_tips->clear();
    knob_name->clear();
    knob_label->clear();
    minimum_edit->clear();
    maximum_edit->clear();
    default_value_edit->clear();
    over_line_check->set_check(false);
    bidimensional_check->set_check(false);
    save_file_dialog_check->set_check(false);
    allowed_file_types->clear();
}

void knob_editor::edit_box_close()
{
    for (action *_action : actions)
        _action->set_checked(false);

    edit_box->hide();
}

void knob_editor::insert_division_to_tabs(QPointF position)
{
    QWidget *__tab_widget = get_tab_widget_under_cursor();
    if (!__tab_widget)
        return;

    current_panel = get_panel_from_widget(__tab_widget);
    if (!current_panel)
    {
        hide_all_dividing_line();
        return;
    }

    tab_widget *_tab_widget = static_cast<tab_widget *>(__tab_widget);

    int index = 0;
    for (tab *_tab : _tab_widget->get_tabs())
    {
        float tab_center_x = _tab->pos().x() + (_tab->size().width() / 2);
        if (position.x() < tab_center_x)
            break;

        index++;
    }

    trim_panel *panel = static_cast<trim_panel *>(current_panel);

    QHBoxLayout *tabs_layout = _tab_widget->get_tab_bar_layout();
    QWidget *dividing_line_v = panel->get_dividing_line_v();

    if (index >= tabs_layout->count())
        tabs_layout->addWidget(dividing_line_v);
    else
        tabs_layout->insertWidget(index + 1, dividing_line_v);

    dividing_line_v->show();

    insert_index = index;
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
    last_knob_under_cursor = nullptr;
}

void knob_editor::dragging_insertion(QPointF pos)
{
    if (!insert_knob_or_tab)
        return;

    if (current_knob_type == "tab" && !dragging_knob)
        insert_division_to_tabs(pos);
    else
        insert_division_to_knobs();
}

void knob_editor::insert_division_to_knobs()
{
    knob *_knob = get_knob_under_cursor();
    QWidget *panel = get_panel_under_cursor();

    // solo se puede arrastrar en el mismo panel del knob a mover
    bool other_panel = false;
    if (dragging_knob && panel)
        other_panel = dragging_knob->get_panel() != panel;
    //

    if (!panel || other_panel)
    {
        hide_all_dividing_line();
        current_panel = nullptr;
        return;
    }

    trim_panel * _panel = static_cast<trim_panel *>(panel);

    QStringList only_read_tabs = _panel->get_only_read_tabs();
    QString current_tab =
        _panel->get_tab_widget()->get_current_tab()->get_name();

    if (only_read_tabs.contains(current_tab))
        return;

    current_panel = panel;
    insert_index = 0;

    if (_knob)
        insert_index = get_index_knob(current_panel, _knob->get_name()) + 1;

    QVBoxLayout *layout = get_layout_current_tab(current_panel);
    if (layout)
    {
        QWidget *dividing_line_h = _panel->get_dividing_line_h();

        layout->insertWidget(insert_index, dividing_line_h);
        dividing_line_h->show();
    }
}

void knob_editor::hide_all_dividing_line()
{
    properties *__properties = static_cast<properties *>(_properties);

    for (QWidget *panel : __properties->get_trim_panels())
    {
        trim_panel *_panel = static_cast<trim_panel *>(panel);
        _panel->get_dividing_line_h()->hide();
        _panel->get_dividing_line_v()->hide();

        _panel->get_dividing_line_h()->setParent(0);
        _panel->get_dividing_line_v()->setParent(0);
    }
}

void knob_editor::delete_knob(knob *_knob, bool cancel_editing_knob)
{
    if (_knob == editing_knob && cancel_editing_knob)
        finish_edit(false);

    trim_panel *panel = static_cast<trim_panel *>(_knob->get_panel());

    // traspasa el espacio inicial, al siguiente knob
    QString next_knob_name =
        disable_over_line_to_next_knob(_knob->get_panel(), _knob->get_name());
    if (!next_knob_name.isEmpty())
    {
        knob *next_knob = panel->get_knob(next_knob_name);
        next_knob->set_init_space(_knob->get_init_space_width());

        QString type = next_knob->get_type();
        if (type == "choice" || type == "floating_dimensions")
            next_knob->set_init_label(true);

        next_knob->set_edit_mode(true);
    }
    //

    panel->remove_custom_knob(_knob->get_name());
}

void knob_editor::edit_tab(tab *_tab)
{
    if (editing_knob)
        editing_knob->set_editing_knob(false);

    editing_tab = _tab;

    knob_name->setText(_tab->get_name());

    edit_label->setText(_tab->get_name() + "' ...");
    edit_icon->set_icon(get_icon_name_from_type("tab"));

    edit_tools->setVisible(true);
    append_tools->setVisible(false);

    update_edit_options_from_type(true, "tab");
}

void knob_editor::edit_knob(knob *_knob)
{
    if (editing_knob)
        editing_knob->set_editing_knob(false);

    editing_knob = _knob;
    _knob->set_editing_knob(true);

    QJsonObject knob_data = _knob->get_knob_data();

    QString name = knob_data.value("name").toString();
    QString type = knob_data.value("type").toString();
    QString tips = knob_data.value("tooltip").toString();
    QString label = knob_data.value("label").toString();
    float min = knob_data.value("minimum").toDouble();
    float max = knob_data.value("maximum").toDouble();
    float default_value = knob_data.value("default").toDouble();
    bool over_line = knob_data.value("over_line").toBool();
    bool bidimensional = knob_data.value("bidimensional").toBool();
    bool save_file_dialog = knob_data.value("save_file_dialog").toBool();

    QString _allowed_file_types;
    for (QJsonValue value : knob_data.value("allowed_file_types").toArray())
        _allowed_file_types += value.toString() + "\n";

    _allowed_file_types =
        _allowed_file_types.left(_allowed_file_types.length() - 1);

    knob_name->setText(name);
    knob_label->setText(label);
    knob_tips->setText(tips);
    minimum_edit->setText(QString::number(min));
    maximum_edit->setText(QString::number(max));
    default_value_edit->setText(QString::number(default_value));
    over_line_check->set_check(over_line);
    bidimensional_check->set_check(bidimensional);
    save_file_dialog_check->set_check(save_file_dialog);
    allowed_file_types->setText(_allowed_file_types);

    edit_label->setText(_knob->get_name() + "' ...");
    edit_icon->set_icon(get_icon_name_from_type(_knob->get_type()));

    edit_tools->setVisible(true);
    append_tools->setVisible(false);

    update_edit_options_from_type(true, _knob->get_type());
}

void knob_editor::finish_edit(bool ok)
{
    if (ok)
    {
        if (editing_knob)
            finish_edit_knob();
        else
            finish_edit_tab();
    }

    edit_box_clear();

    if (editing_knob)
        editing_knob->set_editing_knob(false);

    edit_tools->setVisible(false);
    append_tools->setVisible(true);
    edit_box_close();

    editing_knob = nullptr;
    editing_tab = nullptr;
}

void knob_editor::finish_edit_tab()
{
    if (!editing_tab)
        return;

    trim_panel *panel = static_cast<trim_panel *>(editing_tab->get_panel());

    QString old_name = editing_tab->get_name();

    if (old_name == knob_name->text())
    {
        editing_knob = nullptr;
        return;
    }

    QString new_name = get_available_tab_name(panel, knob_name->text());
    QJsonArray *custom_knobs = panel->custom_knobs;

    QJsonArray custom_knobs_edited;

    for (QJsonValue value : *custom_knobs)
    {
        QJsonObject knob_obj = value.toObject();
        if (knob_obj.value("tab").toString() == old_name)
            knob_obj["tab"] = new_name;

        custom_knobs_edited.push_back(knob_obj);
    }

    *panel->custom_knobs = custom_knobs_edited;

    editing_tab->set_name(new_name);

    editing_tab = nullptr;
}

void knob_editor::finish_edit_knob()
{
    trim_panel *panel = static_cast<trim_panel *>(editing_knob->get_panel());

    QString tab_name = get_custom_tab_name(panel);
    int index = get_index_knob(panel, editing_knob->get_name());

    knob_params params = get_params_from_edit_box(panel);
    params.type = editing_knob->get_type();

    bool keep_name = editing_knob->get_name() == knob_name->text();
    update_knob(editing_knob, params, index, keep_name);

    editing_knob = nullptr;
}

void knob_editor::drag_knob(knob *_knob)
{
    trim_panel *panel = static_cast<trim_panel *>(_knob->get_panel());
    panel->setCursor(Qt::ClosedHandCursor);

    dragging_knob = _knob;
    start_insertion();
}
void knob_editor::drag_knob_move(QWidget *panel)
{
    dragging_insertion();
}

void knob_editor::drag_knob_release(QWidget *_panel)
{
    trim_panel *panel = static_cast<trim_panel *>(_panel);
    panel->setCursor(Qt::ArrowCursor);

    finish_insertion();
}
