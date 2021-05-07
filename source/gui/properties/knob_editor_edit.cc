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

    add_knob(panel);
}

void knob_editor::add_knob(QWidget *panel, int index)
{
    if (index == -2 || !panel)
        return;

    trim_panel *_panel = static_cast<trim_panel *>(panel);

    QString label = knob_name->text();
    if (label.isEmpty())
        label = current_knob_type;

    QString name = get_available_knob_name(panel);
    QString tips = knob_tips->toPlainText();

    float min = 0;
    float max = 1;
    if (!minimum_edit->text().isEmpty())
        min = minimum_edit->text().toDouble();
    if (!maximum_edit->text().isEmpty())
        max = maximum_edit->text().toDouble();

    QString custom_tab = get_custom_tab_name(panel);

    QJsonObject knob_object;
    if (current_knob_type == "floating")
    {
        knob_object = {{"name", name},   {"type", "floating"},
                       {"label", label}, {"tooltip", tips},
                       {"minimum", min}, {"maximum", max},
                       {"default", min}, {"tab", custom_tab}};
    }
    else if (current_knob_type == "integer")
    {
        knob_object = {{"name", name},    {"type", "integer"}, {"label", label},
                       {"tooltip", tips}, {"minimum", min},    {"maximum", max},
                       {"default", min},  {"tab", custom_tab}};
    }
    else if (current_knob_type == "color")
    {
        knob_object = {{"name", name},
                       {"type", "color"},
                       {"label", label},
                       {"tooltip", tips},
                       {"minimum", min},
                       {"maximum", max},
                       {"centered_handler", true},
                       {"tab", custom_tab},
                       {"default", QJsonArray{0, 0, 0, 0}}};
    }
    else if (current_knob_type == "button")
    {
        knob_object = {{"name", name},
                       {"type", "button"},
                       {"label", label},
                       {"tab", custom_tab},
                       {"tooltip", tips}};
    }
    else if (current_knob_type == "choice")
    {
        knob_object = {{"name", name},
                       {"type", "choice"},
                       {"label", label},
                       {"tooltip", tips},
                       {"tab", custom_tab},
                       {"items", QJsonArray{}},
                       {"default", QJsonArray{0, ""}}};
    }
    else if (current_knob_type == "check_box")
    {
        knob_object = {{"name", name},       {"type", "check_box"},
                       {"label", label},     {"tooltip", tips},
                       {"over_line", false}, {"default", false},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "text")
    {
        knob_object = {{"name", name},       {"type", "text"},
                       {"label", label},     {"tooltip", ""},
                       {"over_line", false}, {"default", tips},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "file")
    {
        knob_object = {{"name", name},       {"type", "file"},
                       {"label", label},     {"tooltip", ""},
                       {"over_line", false}, {"default", tips},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "position")
    {
        knob_object = {{"name", name},
                       {"type", "floating_dimensions"},
                       {"label", label},
                       {"tooltip", tips},
                       {"dimensions", 2},
                       {"over_line", false},
                       {"default", QJsonArray{0, 0}},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "label")
    {
        knob_object = {{"name", name},
                       {"type", "label"},
                       {"label", label},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "group")
    {
        knob_object = {{"name", name},   {"type", "group"},
                       {"label", label}, {"open", false},
                       {"knobs", 2},     {"tab", custom_tab}};
    }
    else if (current_knob_type == "separator")
    {
        knob_object = {
            {"name", name}, {"type", "separator"}, {"tab", custom_tab}};
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
}

void knob_editor::insert_knob_in_tab(QJsonArray *knobs, QJsonObject knob_obj,
                                     QString tab_name, int index)
{
    // inserta el knob en el index correcto, ya que todos los 'custom_knobs' van
    // a estar en una misma lista. Separando los knobs en 2 listas una del tab y
    // otra con los demas knobs, se inserta el knob a la lista de tab y luego se
    // unen las 2 listas.

    QJsonArray knobs_from_tab;
    QJsonArray knobs_other_tabs;
    for (QJsonValue value : *knobs)
    {
        if (value.toObject().value("tab").toString() == tab_name)
            knobs_from_tab.push_back(value);
        else
            knobs_other_tabs.push_back(value);
    }

    if (index == -1)
        knobs_from_tab.push_back(knob_obj);
    else
        knobs_from_tab.insert(index, knob_obj);

    // union de knobs
    *knobs = knobs_other_tabs;
    for (QJsonValue value : knobs_from_tab)
        knobs->push_back(value);
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

QString knob_editor::get_available_knob_name(QWidget *panel) const
{
    trim_panel *_panel = static_cast<trim_panel *>(panel);
    QJsonArray &custom_knobs = _panel->custom_knobs;
    QJsonArray &base_knobs = _panel->base_knobs;
    QJsonArray &shared_knobs = _panel->shared_knobs;

    QString name = knob_name->text();
    if (name.isEmpty())
        name = current_knob_type;

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

knob *knob_editor::get_knob_under_cursor() const
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

    knob *_knob = get_knob(14);
    if (!_knob)
        _knob = get_knob(0);

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

    if (_knob)
        insert_index =
            get_index_knob(current_panel, _knob->get_name(), current_tab) + 1;
    else
        insert_index = 0;

    QVBoxLayout *layout = get_layout_current_tab(current_panel);
    if (layout)
    {
        QWidget *dividing_line_h = _panel->get_dividing_line_h();
        layout->insertWidget(insert_index, dividing_line_h);
        dividing_line_h->show();
    }
}

int knob_editor::get_index_knob(QWidget *panel, QString knob_name,
                                QString tab_name) const
{
    if (!panel)
        return -2;

    trim_panel *_panel = static_cast<trim_panel *>(panel);
    QJsonArray &knobs = _panel->custom_knobs;

    // lista de knob que estan en un tab especifico
    QJsonArray knobs_from_tab;
    for (QJsonValue value : knobs)
        if (value.toObject().value("tab").toString() == tab_name)
            knobs_from_tab.push_back(value);

    // encuentra el index del tab
    int index = 0;
    for (QJsonValue value : knobs_from_tab)
    {
        QString _knob_name = value.toObject().value("name").toString();

        if (_knob_name == knob_name)
            return index;

        index++;
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
