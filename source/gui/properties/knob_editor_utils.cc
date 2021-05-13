#include <knob_editor.h>
#include <trim_panel.h>

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

    params.allowed_file_types = allowed_file_types->toPlainText();
    params.save_file_dialog = save_file_dialog_check->is_checked();

    return params;
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
    QJsonArray *custom_knobs = _panel->custom_knobs;
    QJsonArray &base_knobs = _panel->base_knobs;
    QJsonArray &shared_knobs = _panel->shared_knobs;

    QStringList name_list;
    for (QJsonValue value : *custom_knobs)
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

