#include <knob_editor.h>
#include <trim_panel.h>
#include <util.h>

void knob_editor::add_knob()
{
    QString label = knob_label->text();
    QString name = get_available_name();

    if (label.isEmpty())
        label = name;

    QJsonObject knob_object;
    if (current_knob_type == "floating")
    {
        knob_object = {{"name", name},  {"type", "floating"}, {"label", label},
                       {"tooltip", ""}, {"minimum", 0},       {"maximum", 1},
                       {"default", 0.5}};
    }
    else if (current_knob_type == "integer")
    {
        knob_object = {{"name", name},  {"type", "integer"}, {"label", label},
                       {"tooltip", ""}, {"minimum", 0},      {"maximum", 1},
                       {"default", 0.5}};
    }
    else if (current_knob_type == "color")
    {
        knob_object = {{"name", name},
                       {"type", "color"},
                       {"label", label},
                       {"tooltip", ""},
                       {"minimum", 0},
                       {"maximum", 1},
                       {"centered_handler", true},
                       {"default", QJsonArray{0, 0, 0, 0}}};
    }
    else if (current_knob_type == "button")
    {
        knob_object = {{"name", name},
                       {"type", "button"},
                       {"label", label},
                       {"tooltip", ""}};
    }
    else if (current_knob_type == "choice")
    {
        knob_object = {{"name", name},          {"type", "choice"},
                       {"label", label},        {"tooltip", ""},
                       {"items", QJsonArray{}}, {"default", QJsonArray{0, ""}}};
    }
    else if (current_knob_type == "check_box")
    {
        knob_object = {{"name", name},       {"type", "check_box"},
                       {"label", label},     {"tooltip", ""},
                       {"over_line", false}, {"default", false}};
    }
    else if (current_knob_type == "text")
    {
        knob_object = {{"name", name},  {"type", "text"},     {"label", label},
                       {"tooltip", ""}, {"over_line", false}, {"default", ""}};
    }
    else if (current_knob_type == "file")
    {
        knob_object = {{"name", name},  {"type", "file"},     {"label", label},
                       {"tooltip", ""}, {"over_line", false}, {"default", ""}};
    }
    else if (current_knob_type == "floating_dimensions")
    {
        knob_object = {{"name", name},
                       {"type", "floating_dimensions"},
                       {"label", label},
                       {"tooltip", ""},
                       {"dimensions", 2},
                       {"over_line", false},
                       {"default", QJsonArray{0, 0}}};
    }
    else if (current_knob_type == "label")
    {
        knob_object = {{"name", name}, {"type", "label"}, {"label", label}};
    }
    else if (current_knob_type == "group")
    {
        knob_object = {{"name", name},
                       {"type", "group"},
                       {"label", label},
                       {"open", false},
                       {"knobs", 2}};
    }
    else if (current_knob_type == "separator")
    {
        knob_object = {{"name", name}, {"type", "separator"}};
    }

    if (knob_object.empty())
        return;

    knobs.push_back(knob_object);
    static_cast<trim_panel *>(panel)->update_controls_knobs(knobs);
}

QString knob_editor::get_available_name() const
{
    QString name = knob_name->text();
    if (name.isEmpty())
        name = current_knob_type;

    auto exist = [this](QString _name) {
        for (QJsonValue value : knobs)
            if (value.toObject().value("name").toString() == _name)
                return true;

        return false;
    };

    auto get_basename = [](QString name) {
        QString str;
        for (QChar letter : name)
            if (!letter.isDigit())
                str += letter;
        return str;
    };

    QString basename = get_basename(name);
    QString available_name;

    int number = 1;
    while (true)
    {
        available_name = basename + QString::number(number);
        if (!exist(available_name))
            break;
        number++;
    }

    return available_name;
}

knob *knob_editor::get_knob_under_cursor() const
{
    int vertical_gap = -15;

    QWidget *_widget =
        qApp->widgetAt({QCursor::pos().x(), QCursor::pos().y() + vertical_gap});
    while (_widget)
    {
        QString name = _widget->objectName();
        knob *_knob = dynamic_cast<knob *>(_widget);
        if (_knob)
            return _knob;

        _widget = _widget->parentWidget();
    }

    return nullptr;
}

int knob_editor::get_index_knob(QString knob_name) const
{
    int index = 0;
    for (QJsonValue value : knobs)
    {
        if (value.toObject().value("name").toString() == knob_name)
            return index;

        index++;
    }

    return index;
}

QVBoxLayout *knob_editor::get_controls_layout() const
{
    QLayout *layout =
        static_cast<trim_panel *>(panel)->get_controls_tab()->layout();

    return static_cast<QVBoxLayout *>(layout);
}

void knob_editor::mousePressEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ClosedHandCursor);
}

void knob_editor::mouseMoveEvent(QMouseEvent *event)
{
    knob *_knob = get_knob_under_cursor();

    if (_knob)
    {
        int index = get_index_knob(_knob->get_name()) + 2;
        get_controls_layout()->insertWidget(index, temp_widget);
        temp_widget->show();
    }
}

void knob_editor::mouseReleaseEvent(QMouseEvent *event)
{
    temp_widget->hide();
    this->setCursor(Qt::ArrowCursor);
}

