#include <knob_color.hpp>

knob_color::knob_color(
    int _init_space_width,
    QString _label,
    QColor _default_value)
{
    init_space_width = _init_space_width;
    label = _label;
    default_value = _default_value;

    this->setObjectName("knob_color");

    setup_ui();
}

knob_color::~knob_color()
{
}

void knob_color::setup_ui()
{

    int icon_size = 20;

    QHBoxLayout *layout = new QHBoxLayout(this);

    init_space *label_widget = new init_space(init_space_width, label);
    layout->addWidget(label_widget);

    value = new QLineEdit(QString::number(default_value.red()));
    value->setMaximumWidth(50);
    layout->addWidget(value);

    _slider = new slider();
    layout->addWidget(_slider);

    _separate_colors_slider = separate_colors_slider();
    _separate_colors_slider->hide();
    layout->addWidget(_separate_colors_slider);

    _separate_colors_box = separate_colors_box();
    _separate_colors_box->hide();
    layout->addWidget(_separate_colors_box);

    //
    //

    QPushButton *pick = new QPushButton();
    layout->addWidget(pick);

    QPushButton *picker = new QPushButton();
    connect(picker, &QPushButton::clicked, this, [this]() {
        is_separate_colors_slider = !is_separate_colors_slider;

        _slider->setVisible(!is_separate_colors_slider);
        value->setVisible(!is_separate_colors_slider);
        _separate_colors_slider->setVisible(is_separate_colors_slider);
    });

    qt::set_icon(picker, "color_a", icon_size);
    layout->addWidget(picker);

    QPushButton *switch_color = new QPushButton("4");
    connect(switch_color, &QPushButton::clicked, this, [this]() {
        is_separate_colors_box = !is_separate_colors_box;

        _slider->setVisible(!is_separate_colors_box);
        value->setVisible(!is_separate_colors_box);
        _separate_colors_box->setVisible(is_separate_colors_box);
    });
    layout->addWidget(switch_color);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", icon_size);
    layout->addWidget(animation);
}

QWidget *knob_color::separate_colors_box()
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setMargin(0);

    QLineEdit *red = new QLineEdit();
    red->setText("1");
    layout->addWidget(red);

    QLineEdit *green = new QLineEdit();
    green->setText("0.5");
    layout->addWidget(green);

    QLineEdit *blue = new QLineEdit();
    blue->setText("1");
    layout->addWidget(blue);

    QLineEdit *alpha = new QLineEdit();
    alpha->setText("0");
    layout->addWidget(alpha);

    return widget;
}

QWidget *knob_color::separate_colors_slider()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setMargin(0);

    slider *red = new slider();
    layout->addWidget(red);

    slider *green = new slider();
    layout->addWidget(green);

    slider *blue = new slider();
    layout->addWidget(blue);

    slider *alpha = new slider();
    layout->addWidget(alpha);

    return widget;
}
