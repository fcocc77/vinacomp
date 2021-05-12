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
        knob_object = {{"name", params.name},    {"type", "text"},
                       {"label", params.label},  {"tooltip", ""},
                       {"default", params.tips}, {"tab", custom_tab}};
    }
    else if (params.type == "file")
    {
        knob_object = {{"name", params.name},   {"type", "file"},
                       {"label", params.label}, {"tooltip", params.tips},
                       {"default", ""},         {"tab", custom_tab}};
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

    QJsonArray &knobs = _panel->custom_knobs;

    if (index == -1)
        insert_knob_in_tab(&knobs, knob_object, custom_tab);
    else
    {
        if (knobs.count() < index)
            return;
        insert_knob_in_tab(&knobs, knob_object, custom_tab, index);
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
        finish_edit_knob(false);

    QJsonObject knob_data = dragging_knob->get_knob_data();

    knob_params params;
    QString name = knob_data.value("name").toString();
    params.name = get_available_knob_name(panel, name);

    params.type = knob_data.value("type").toString();
    params.tips = knob_data.value("tooltip").toString();
    params.label = knob_data.value("label").toString();
    params.min = knob_data.value("minimum").toDouble();
    params.max = knob_data.value("maximum").toDouble();
    params.default_value = knob_data.value("default").toDouble();

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
    //

    delete_knob(dragging_knob);
    add_knob(panel, params, index);

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
    for (int i = 0; i < _panel->custom_knobs.count(); i++)
    {
        QJsonObject __knob = _panel->custom_knobs[i].toObject();
        QString __knob_name = __knob.value("name").toString();
        if (__knob_name == next_knob_name)
        {
            __knob["over_line"] = false;
            _panel->custom_knobs[i] = __knob;
        }
    }
    //

    return next_knob_name;
}

QList<knob *> knob_editor::get_line_widget_knobs(QWidget *_panel,
                                                 QString knob_name) const
{
    // obtiene una lista de todos los knob hermanos de widget 'over_line'
    trim_panel *panel = static_cast<trim_panel *>(_panel);
    knob *_knob = panel->get_knob(knob_name);

    if (!_knob)
        return {};

    QWidget *line_widget = _knob->get_over_line_widget();
    if (!line_widget)
        return {};

    QList<knob *> list_knobs;

    for (int i = 0; i < line_widget->layout()->count(); i++)
    {
        QLayoutItem *item = line_widget->layout()->itemAt(i);
        knob *__knob = dynamic_cast<knob *>(item->widget());
        if (__knob)
            list_knobs.push_back(__knob);
    }

    return list_knobs;
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

QList<QList<QJsonObject>>
knob_editor::get_knobs_by_tab(QJsonArray knobs, QString tab_name,
                              QJsonArray *leftover_knobs) const
{
    QJsonArray knobs_other_tabs;
    QList<QList<QJsonObject>> knobs_from_tab;

    for (QJsonValue value : knobs)
    {
        if (value.toObject().value("tab").toString() == tab_name)
        {
            bool over_line = value.toObject().value("over_line").toBool();

            if (over_line && !knobs_from_tab.empty())
            {
                knobs_from_tab.last().push_back(value.toObject());
                continue;
            }

            QList<QJsonObject> line_knobs = {value.toObject()};
            knobs_from_tab.push_back(line_knobs);
        }
        else if (leftover_knobs)
            leftover_knobs->push_back(value);
    }

    return knobs_from_tab;
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

knob_params knob_editor::get_params_from_edit_box(QWidget *panel) const
{
    if (!panel)
        return {};

    knob_params params;

    QString label = knob_label->text();
    if (label.isEmpty())
    {
        label = knob_name->text();
        if (label.isEmpty())
            label = current_knob_type;
    }
    params.label = label;

    QString name = knob_name->text();
    if (name.isEmpty())
        name = label;

    params.name = get_available_knob_name(panel, name);
    params.tips = knob_tips->toPlainText();

    params.min = 0;
    params.max = 1;
    if (!minimum_edit->text().isEmpty())
        params.min = minimum_edit->text().toDouble();
    if (!maximum_edit->text().isEmpty())
        params.max = maximum_edit->text().toDouble();

    QString default_value = default_value_edit->text();
    if (default_value.isEmpty())
        params.default_value = params.min;
    else
        params.default_value = default_value.toDouble();

    params.over_line = over_line_check->is_checked();
    params.bidimensional = bidimensional_check->is_checked();

    params.type = current_knob_type;

    return params;
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
}

void knob_editor::edit_box_close()
{
    for (action *_action : actions)
        _action->set_checked(false);

    edit_box->hide();
}

QString knob_editor::get_available_tab_name(QWidget *panel, QString preferred_name) const
{
    tab_widget *_tab_widget =
        static_cast<trim_panel *>(panel)->get_tab_widget();

    QString name = knob_name->text();
    if (name.isEmpty())
        name = current_knob_type;

    if (!preferred_name.isEmpty())
        name = preferred_name;

    QStringList name_list;
    for (tab *_tab : _tab_widget->get_tabs())
        name_list.push_back(_tab->get_name());

    return get_available_name(name_list, name);
}

QString knob_editor::get_available_knob_name(QWidget *panel, QString name) const
{
    trim_panel *_panel = static_cast<trim_panel *>(panel);
    QJsonArray &custom_knobs = _panel->custom_knobs;
    QJsonArray &base_knobs = _panel->base_knobs;
    QJsonArray &shared_knobs = _panel->shared_knobs;

    QStringList name_list;
    for (QJsonValue value : custom_knobs)
        name_list.push_back(value.toObject().value("name").toString());

    for (QJsonValue value : base_knobs)
        name_list.push_back(value.toObject().value("name").toString());

    for (QJsonValue value : shared_knobs)
        name_list.push_back(value.toObject().value("name").toString());

    name = name.replace(" ", "_").toLower();

    return get_available_name(name_list, name);
}

QString knob_editor::get_available_name(QStringList name_list, QString name) const
{
    auto get_basename = [](QString name) {
        QString str;
        for (QChar letter : name)
            if (!letter.isDigit())
                str += letter;
        return str;
    };

    QString basename = get_basename(name);
    QString available_name = basename;

    int number = 1;
    while (true)
    {
        if (!name_list.contains(available_name))
            break;

        available_name = basename + QString::number(number);
        number++;
    }

    return available_name;
}

QString knob_editor::get_custom_tab_name(QWidget *panel)
{
    // busca el tab actual, y si el un tab que los knobs de base, crea un tab
    // nuevo para agregar custom knobs

    trim_panel *_panel = static_cast<trim_panel *>(panel);
    tab_widget *_tab_widget = _panel->get_tab_widget();

    QString tab_name = _tab_widget->get_current_tab()->get_name();
    QStringList only_read_tabs = _panel->get_only_read_tabs();

    if (!only_read_tabs.contains(tab_name))
        return tab_name;

    // busca un tab 'custom' que exista
    for (tab *_tab : _tab_widget->get_tabs())
    {
        tab_name = _tab->get_name();
        if (!only_read_tabs.contains(tab_name))
        {
            _tab_widget->set_tab(tab_name);
            return tab_name;
        }
    }

    tab_name = add_tab(panel, -1, "Custom");

    return tab_name;
}

knob *knob_editor::get_knob_under_cursor()
{
    auto get_knob = [=](int gap) {
        QPoint pos = QCursor::pos();
        QWidget *_widget = qApp->widgetAt({pos.x(), pos.y() - gap});

        knob *_knob = nullptr;
        while (_widget)
        {
            _knob = dynamic_cast<knob *>(_widget);
            if (_knob)
                return _knob;

            _widget = _widget->parentWidget();
        }

        return _knob;
    };

    // detecta si el over cursor es el primer 'knob' si es 'over_line'
    // utiliza el padre 'line_widget' para comparar el 'y()'
    bool is_first = false;
    knob *first_knob = get_knob(-14);
    if (first_knob)
    {
        QWidget *widget = first_knob->get_over_line_widget();
        if (!widget)
            widget = first_knob;

        if (widget->y() < 25)
            is_first = true;
    }

    if (is_first)
    {
        last_knob_under_cursor = nullptr;
        return nullptr;
    }
    //
    //

    knob *_knob = get_knob(14);
    if (!_knob)
        _knob = get_knob(0);

    if (!_knob)
        _knob = last_knob_under_cursor;

    if (_knob)
        last_knob_under_cursor = _knob;

    return _knob;
}

QWidget *knob_editor::get_panel_under_cursor() const
{
    QWidget *_widget = qApp->widgetAt(QCursor::pos());
    return get_panel_from_widget(_widget);
}

QWidget *knob_editor::get_panel_from_widget(QWidget *widget) const
{
    while (widget)
    {
        trim_panel *panel = dynamic_cast<trim_panel *>(widget);
        if (panel)
            return panel;

        widget = widget->parentWidget();
    }

    return nullptr;
}

QWidget *knob_editor::get_tab_widget_under_cursor() const
{
    QWidget *_widget = qApp->widgetAt(QCursor::pos());
    while (_widget)
    {
        if (_widget->objectName() == "tab_widget")
            return _widget;

        _widget = _widget->parentWidget();
    }

    return nullptr;

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

void knob_editor::insert_division_to_knobs()
{
    knob *_knob = get_knob_under_cursor();
    QWidget *panel = get_panel_under_cursor();

    if (!panel)
    {
        hide_all_dividing_line();
        current_panel = nullptr;
        return;
    }

    trim_panel *_panel = static_cast<trim_panel *>(panel);

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

int knob_editor::get_index_knob(QWidget *panel, QString knob_name) const
{
    if (!panel)
        return -2;

    QList<QList<knob *>> lines_knobs;

    // crea una lista de listas de 'knobs' asi incluir los knobs que estan en
    // 'over_line'
    QVBoxLayout *layout = get_layout_current_tab(panel);
    for (int i = 0; i < layout->count(); i++)
    {
        QLayoutItem *item = layout->itemAt(i);
        QWidget *item_widget = item->widget();

        if (!item_widget)
            continue;

        knob *_knob = dynamic_cast<knob *>(item_widget);

        if (_knob)
            lines_knobs.push_back({_knob});
        else if (item_widget->objectName() == "line_widget")
        {
            QList<knob *> line_widget_knobs;

            QLayout *line_layout = item_widget->layout();
            for (int i2 = 0; i2 < line_layout->count(); i2++)
            {
                QWidget *item_widget2 = line_layout->itemAt(i2)->widget();
                knob *_knob = dynamic_cast<knob *>(item_widget2);
                if (_knob)
                    line_widget_knobs.push_back(_knob);
            }

            lines_knobs.push_back(line_widget_knobs);
        }
    }

    // encuentra el knob y retorna el index de la linea en la que esta el 'knob'
    for (int i = 0; i < lines_knobs.count(); i++)
    {
        for (knob *_knob : lines_knobs.value(i))
            if (_knob->get_name() == knob_name)
                return i;
    }

    return -1;
}

QVBoxLayout *knob_editor::get_layout_current_tab(QWidget *panel) const
{
    if (!panel)
        return nullptr;

    tab_widget *_tab_widget =
        static_cast<trim_panel *>(panel)->get_tab_widget();

    QLayout *layout = _tab_widget->get_current_tab()->get_widget()->layout();

    return static_cast<QVBoxLayout *>(layout);
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
        finish_edit_knob(false);

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

    knob_name->setText(name);
    knob_label->setText(label);
    knob_tips->setText(tips);
    minimum_edit->setText(QString::number(min));
    maximum_edit->setText(QString::number(max));
    default_value_edit->setText(QString::number(default_value));
    over_line_check->set_check(over_line);
    bidimensional_check->set_check(bidimensional);

    edit_label->setText(_knob->get_name() + "' ...");
    edit_icon->set_icon(get_icon_name_from_type(_knob->get_type()));

    edit_tools->setVisible(true);
    append_tools->setVisible(false);

    update_edit_options_from_type(true, _knob->get_type());
}

void knob_editor::finish_edit_knob(bool ok)
{
    if (ok)
    {
        QWidget *panel = editing_knob->get_panel();
        QString tab_name = get_custom_tab_name(panel);
        int index = get_index_knob(panel, editing_knob->get_name());

        knob_params params = get_params_from_edit_box(panel);
        params.type = editing_knob->get_type();

        delete_knob(editing_knob, false);
        add_knob(panel, params, index);

        editing_knob = nullptr;
    }

    edit_box_clear();

    if (editing_knob)
        editing_knob->set_editing_knob(false);

    edit_tools->setVisible(false);
    append_tools->setVisible(true);
    edit_box_close();

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
