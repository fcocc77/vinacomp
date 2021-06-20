#include <knob_slider.h>
#include <vinacomp.h>
#include "node.h"
#include <animation.h>
#include <trim_panel.h>

knob_slider::knob_slider(knob_props props, float min, float max,
                         float _default_value, bool _bidimensional,
                         bool floating, bool centered_handler)
    : knob(props)
    , value_2_edit(nullptr)
    , dimensions(false)
    , bidimensional(_bidimensional)
    , show_dimensions(nullptr)
    , empty_widget(nullptr)
    , default_value(_default_value)

{
    this->setObjectName("knob_slider");
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(init_space);
    this->set_knob_layout(layout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (_bidimensional)
        knob::set_dimensions(2);

    QMenu *curve_menu = nullptr;
    if (props.panel)
        curve_menu = static_cast<trim_panel *>(props.panel)->get_curve_menu();

    // value 1
    value_1_edit = new line_edit(this, 0);
    value_1_edit->set_menu(curve_menu);
    connect(value_1_edit, &line_edit::editingFinished, this, [=]() {
        values.first = value_1_edit->text().toDouble();

        if (!floating)
            values.first = round(values.first);

        set_value(values.first, 0);
    });

    value_1_edit->setMaximumWidth(50);

    // slider
    _slider = new slider(min, max, default_value, floating, centered_handler);
    connect(_slider, &slider::moved, this, [=](float value) {
        values = {value, value};

        value_1_edit->set_clamp_value(value);
        if (value_2_edit)
            value_2_edit->setText(QString::number(value));

        to_emmit_signal();
    });

    layout->addWidget(value_1_edit);

    if (bidimensional)
    {
        // value 2
        value_2_edit = new line_edit(this, 1);
        value_2_edit->set_menu(curve_menu);
        value_2_edit->hide();

        connect(value_2_edit, &line_edit::editingFinished, this, [=]() {
            values.second = value_2_edit->text().toDouble();
            if (!floating)
                values.second = round(values.second);

            set_value(values.second, 1);
        });
        value_2_edit->setMaximumWidth(50);

        // buton de dimensiones
        show_dimensions = new button();
        show_dimensions->setText("2");
        connect(show_dimensions, &QPushButton::clicked, this,
                [=]() { separate_dimensions(!dimensions); });

        empty_widget = new QWidget();
        empty_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        empty_widget->hide();

        layout->addWidget(value_2_edit);
        layout->addWidget(empty_widget);
    }

    layout->addWidget(_slider);
    if (bidimensional)
        layout->addWidget(show_dimensions);

    set_value(default_value, 0, false);
    set_value(default_value, 1, false);

}

knob_slider::~knob_slider()
{
    delete value_1_edit;
    delete _slider;
    delete layout;
    if (bidimensional)
    {
        delete value_2_edit;
        delete empty_widget;
    }
}

void knob_slider::restore_default()
{
    knob::restore_default();
    set_animated(false);
    set_value(default_value, 0, false);
    set_value(default_value, 1, false);
    separate_dimensions(false);
}

void knob_slider::restore_param()
{
    knob::restore_param();

    QJsonArray param_value = get_param_value().toArray();

    QJsonValue _value_1 = param_value[0];
    QJsonValue _value_2 = param_value[1];


    float value_1, value_2;
    if (animated)
    {
        value_1 = anim::get_value(_value_1.toString(), project->frame);
        value_2 = anim::get_value(_value_2.toString(), project->frame);
    }
    else
    {
        value_1 = _value_1.toDouble();
        value_2 = _value_2.toDouble();
    }

    set_values({value_1, value_2}, false);
}

void knob_slider::set_animated(bool animated, int dimension)
{
    if (dimension == -1)
    {
        qt::set_property(value_1_edit, "animated", animated);
        if (bidimensional)
            qt::set_property(value_2_edit, "animated", animated);
    }
    else if (dimension == 0)
        qt::set_property(value_1_edit, "animated", animated);
    else if (dimension == 1)
        qt::set_property(value_2_edit, "animated", animated);

    knob::set_animated(animated, dimension);
}

void knob_slider::set_disable(bool disable)
{
    value_1_edit->setDisabled(disable);
    if (value_2_edit)
        value_2_edit->setDisabled(disable);

    _slider->set_disable(disable);

    qt::set_property(value_1_edit, "disable", disable);

    knob::set_disable(disable);
}

void knob_slider::set_error(bool error)
{
    qt::set_property(value_1_edit, "error", error);
    if (error)
        value_1_edit->setText("Error");
}

void knob_slider::set_has_expression(bool expression)
{
    qt::set_property(value_1_edit, "expression", expression);
    _slider->set_disable(expression);
}

void knob_slider::to_emmit_signal()
{
    update_handler();
    changed(values.first, values.second); // Signal

    if (bidimensional)
        update_value(QJsonArray{values.first, values.second});
    else
        update_value(QJsonArray{values.first});

    update_linked_knobs();
    to_node_panel(this);
}

void knob_slider::update_linked_knobs()
{
    // actualiza los knob linkeados
    for (knob *_knob : get_slaves_knobs())
    {
        knob_slider *_knob_slider = static_cast<knob_slider *>(_knob);
        _knob_slider->set_values(values, false);
    }
}

void knob_slider::update_animated()
{
    int frame = project->frame;

    QString curve = get_curve();
    bool keyframe = false;
    float value = anim::get_value(curve, frame, &keyframe);

    qt::set_property(value_1_edit, "keyframe", keyframe);
    set_value(value, 0, false);
}

void knob_slider::separate_dimensions(bool separate)
{
    if (separate == dimensions || !bidimensional)
        return;

    qt::set_property(show_dimensions, "active", separate);

    _slider->setVisible(!separate);
    value_2_edit->setVisible(separate);
    empty_widget->setVisible(separate);

    dimensions = separate;

    // si las dimensiones no son separadas, calcula el promedio de las 2
    // dimensiones
    if (!separate)
    {
        float average = (values.first + values.second) / 2;
        values = {average, average};
        value_1_edit->set_clamp_value(average);
        value_2_edit->set_clamp_value(average);
        _slider->set_value(average);
    }
}

void knob_slider::set_value(float value, int dimension, bool emmit_signal)
{
    if (!bidimensional && dimension >= 1)
        return;

    if (dimension == 0)
    {
        values.first = value;
        value_1_edit->set_clamp_value(value);
        _slider->set_value(value);
    }
    else
    {
        values.second = value;
        value_2_edit->set_clamp_value(value);
    }

    if (emmit_signal)
        to_emmit_signal();
}

void knob_slider::set_values(pair<float, float> _values, bool emmit_signal)
{
    values = _values;

    separate_dimensions(values.first != values.second);

    value_1_edit->setText(QString::number(values.first));
    _slider->set_value(_values.first);

    if (value_2_edit)
        value_2_edit->setText(QString::number(values.second));

    if (emmit_signal)
        to_emmit_signal();
}

float knob_slider::get_value(int dimension) const
{
    if (dimension == 0)
        return values.first;
    else
        return values.second;
}
