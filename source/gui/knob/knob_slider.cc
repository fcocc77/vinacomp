#include <animation.h>
#include <knob_slider.h>

knob_slider::knob_slider(knob_props props, float min, float max,
                         float default_value, bool _bidimensional,
                         bool floating, bool centered_handler)
    : knob(props)
    , value_2_edit(nullptr)
    , dimensions(false)
    , bidimensional(_bidimensional)
    , show_dimensions(nullptr)
    , emmit_signal(true)
    , empty_widget(nullptr)

{
    this->setObjectName("knob_slider");
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(init_space);
    this->set_knob_layout(layout);

    // value 1
    value_1_edit = new line_edit();
    value_1_edit->set_menu(knob::menu);
    connect(value_1_edit, &line_edit::editingFinished, this, [=]() {
        values.first = value_1_edit->text().toDouble();
        if (!floating)
            values.first = round(values.first);
        set_value_internal(values.first, 0);
    });

    value_1_edit->setMaximumWidth(50);

    // slider
    _slider = new slider(min, max, default_value, floating, centered_handler);
    connect(_slider, &slider::moved, this, [=](float value) {
        values = {value, value};

        value_1_edit->setText(QString::number(value));
        if (value_2_edit)
            value_2_edit->setText(QString::number(value));

        to_emmit_signal();
        emmit_signal = true;
    });

    layout->addWidget(value_1_edit);

    if (bidimensional)
    {
        // value 2
        value_2_edit = new line_edit();
        value_2_edit->set_menu(knob::menu);
        value_2_edit->hide();

        connect(value_2_edit, &line_edit::editingFinished, this, [=]() {
            values.second = value_2_edit->text().toDouble();
            if (!floating)
                values.second = round(values.second);
            set_value_internal(values.second, 1);
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

    set_value(default_value);
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

void knob_slider::restore_param()
{
    knob::restore_param();

    QJsonValue param_value = get_param_value();
    float value;

    if (animated)
        value = anim::get_value(param_value.toString(), project->frame);
    else
        value = param_value.toDouble();

    set_value(value);
}

void knob_slider::set_animated(bool animated)
{
    qt::set_property(value_1_edit, "animated", animated);
    knob::set_animated(animated);
}

void knob_slider::to_emmit_signal()
{
    if (!emmit_signal)
        return;

    update_handler();
    changed(values.first, values.second); // Signal
    update_value(values.first);
}

void knob_slider::update_animated()
{
    int frame = project->frame;

    QString curve = get_param_value().toString();
    bool keyframe = false;
    float value = anim::get_value(curve, frame, &keyframe);

    qt::set_property(value_1_edit, "keyframe", keyframe);
    set_value(value, 0, false);
}

void knob_slider::set_value_internal(float value, int dimension)
{
    if (dimensions)
    {
        if (dimension == 0)
            value_1_edit->setText(QString::number(value));
        else
            value_2_edit->setText(QString::number(value));
    }
    else
        set_value(value, dimension, true);

    to_emmit_signal();
}

void knob_slider::separate_dimensions(bool separate)
{
    if (separate == dimensions)
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
        value_1_edit->setText(QString::number(average));
        value_2_edit->setText(QString::number(average));
        _slider->set_value(average);
    }
}

void knob_slider::set_value(float value, int dimension, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;

    if (dimension == 0)
    {
        values.first = value;
        value_1_edit->setText(QString::number(value));
        _slider->set_value(value);
    }
    else
    {
        values.second = value;
        value_2_edit->setText(QString::number(value));
    }
}

void knob_slider::set_values(pair<float, float> _values, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;
    values = _values;

    separate_dimensions(values.first != values.second);

    value_1_edit->setText(QString::number(values.first));
    _slider->set_value(_values.first);

    value_2_edit->setText(QString::number(values.second));
}

float knob_slider::get_value(int dimension) const
{
    if (dimension == 0)
        return values.first;
    else
        return values.second;
}
