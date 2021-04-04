#include <knob_floating.h>

knob_floating::knob_floating(float min, float max, float default_value, bool _two_dimensional)
    : emmit_signal(true)
    , dimensions(false)
    , two_dimensional(_two_dimensional)
    , show_dimensions(nullptr)
    , value_2_edit(nullptr)
    , empty_widget(nullptr)

{
    this->setObjectName("knob_floating");
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(init_space);
    this->set_knob_layout(layout);

    // value 1
    value_1_edit = new QLineEdit();
    connect(value_1_edit, &QLineEdit::editingFinished, this, [=]() {
        values.first = value_1_edit->text().toDouble();
        set_value_internal(values.first, 0);
    });
    value_1_edit->setMaximumWidth(50);

    // slider
    _slider = new slider(min, max);
    connect(_slider, &slider::moved, this, [=](float value) {
        values = {value, value};

        value_1_edit->setText(QString::number(value));
        if (value_2_edit)
            value_2_edit->setText(QString::number(value));

        if (emmit_signal)
        {
            update_handler();
            changed(value); // Signal
        }
    });

    layout->addWidget(value_1_edit);

    if (two_dimensional)
    {
        // value 2
        value_2_edit = new QLineEdit();
        value_2_edit->hide();

        connect(value_2_edit, &QLineEdit::editingFinished, this, [=]() {
            values.second = value_2_edit->text().toDouble();
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
    if (two_dimensional)
        layout->addWidget(show_dimensions);

    set_value(default_value);
}

knob_floating::~knob_floating()
{
    delete value_1_edit;
    delete _slider;
    delete layout;
    if (two_dimensional)
    {
        delete value_2_edit;
        delete empty_widget;
    }
}

void knob_floating::set_animated(bool animated)
{
    qt::set_property(value_1_edit, "animated", animated);
    knob::set_animated(animated);
}

void knob_floating::set_value_internal(float value, int dimension)
{
    // este set value, ajusta el valor pero sin llamar al slider, ya que esta
    // emite otra señal 'changed' y 'update_handler'
    if (dimensions)
    {
        if (dimension == 0)
            value_1_edit->setText(QString::number(value));
        else
            value_2_edit->setText(QString::number(value));

        update_handler();
        changed(values.first); // Signal
    }
    else
        set_value(value, dimension);
}

void knob_floating::separate_dimensions(bool separate)
{
    if (separate == dimensions)
        return;

    qt::set_property(show_dimensions, "active", separate);

    _slider->setVisible(!separate);
    value_2_edit->setVisible(separate);
    empty_widget->setVisible(separate);

    dimensions = separate;

    // si las dimensiones no son separadas, calcula el promedio de las 2 dimensiones
    if (!separate)
    {
        float average = (values.first + values.second) / 2;
        values = {average, average};
        value_1_edit->setText(QString::number(average));
        value_2_edit->setText(QString::number(average));
        _slider->set_value(average);
    }
}

void knob_floating::set_value(float value, int dimension, bool _emmit_signal)
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

void knob_floating::set_values(pair<float, float> _values, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;
    values = _values;

    separate_dimensions(values.first != values.second);

    value_1_edit->setText(QString::number(values.first));
    _slider->set_value(_values.first, false);

    value_2_edit->setText(QString::number(values.second));
}

float knob_floating::get_value(int dimension) const
{
    if (dimension == 0)
        return values.first;
    else
        return values.second;
}
