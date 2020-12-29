#include <knob_color.h>

knob_color::knob_color(
    QColor _default_value)
{
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

    // init_space *label_widget = new init_space(init_space_width, label);
    layout->addWidget(init_space);

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
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setMargin(0);

	// Red
	QWidget *red_widget = new QWidget(this);
    QHBoxLayout *red_layout = new QHBoxLayout(red_widget);
	red_layout->setMargin(0);

    QLineEdit *red_edit = new QLineEdit(this);
    red_edit->setMaximumWidth(50);
    slider *red_slider = new slider();

	red_layout->addWidget(red_edit);
	red_layout->addWidget(red_slider);
    layout->addWidget(red_widget);
	//

	// Green
	QWidget *green_widget = new QWidget(this);
    QHBoxLayout *green_layout = new QHBoxLayout(green_widget);
	green_layout->setMargin(0);

    QLineEdit *green_edit = new QLineEdit(this);
    green_edit->setMaximumWidth(50);
    slider *green_slider = new slider();

	green_layout->addWidget(green_edit);
	green_layout->addWidget(green_slider);
    layout->addWidget(green_widget);
	//

	// Blue
	QWidget *blue_widget = new QWidget(this);
	QHBoxLayout *blue_layout = new QHBoxLayout(blue_widget);
	blue_layout->setMargin(0);

	QLineEdit *blue_edit = new QLineEdit(this);
	blue_edit->setMaximumWidth(50);
	slider *blue_slider = new slider();

	blue_layout->addWidget(blue_edit);
	blue_layout->addWidget(blue_slider);
	layout->addWidget(blue_widget);
	//

	// Alpha
	QWidget *alpha_widget = new QWidget(this);
    QHBoxLayout *alpha_layout = new QHBoxLayout(alpha_widget);
	alpha_layout->setMargin(0);

    QLineEdit *alpha_edit = new QLineEdit(this);
    alpha_edit->setMaximumWidth(50);
    slider *alpha_slider = new slider();

	alpha_layout->addWidget(alpha_edit);
	alpha_layout->addWidget(alpha_slider);
    layout->addWidget(alpha_widget);
	//


    return widget;
}
