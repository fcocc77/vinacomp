#include <knob_color.h>

void knob_color::setup_ui()
{
    int icon_size = 20;

    QHBoxLayout *layout = new QHBoxLayout(this);

    // init_space *label_widget = new init_space(init_space_width, label);
    layout->addWidget(init_space);

    value_edit = new QLineEdit(QString::number(red));
    value_edit->setMaximumWidth(50);
    layout->addWidget(value_edit);

    _slider = new slider();
	connect(_slider, &slider::moved, this, [=](float value){
		value_edit->setText(QString::number(value));
	});
    layout->addWidget(_slider);

    _separate_colors_slider = separate_colors_slider();
    _separate_colors_slider->hide();
    layout->addWidget(_separate_colors_slider);

    _separate_colors_box = separate_colors_box();
    _separate_colors_box->hide();
    layout->addWidget(_separate_colors_box);

    //
    //

	picker_button = new QPushButton();
	picker_button->setAutoFillBackground(true);
    layout->addWidget(picker_button);

    QPushButton *picker = new QPushButton();
    connect(picker, &QPushButton::clicked, this, [this]() {
        is_separate_colors_slider = !is_separate_colors_slider;

        _slider->setVisible(!is_separate_colors_slider);
        value_edit->setVisible(!is_separate_colors_slider);
        _separate_colors_slider->setVisible(is_separate_colors_slider);

		update();
    });

    qt::set_icon(picker, "color_a", icon_size);
    layout->addWidget(picker);

    QPushButton *switch_color = new QPushButton("4");
    connect(switch_color, &QPushButton::clicked, this, [this]() {
        is_separate_colors_box = !is_separate_colors_box;

        _slider->setVisible(!is_separate_colors_box);
        value_edit->setVisible(!is_separate_colors_box);
        _separate_colors_box->setVisible(is_separate_colors_box);

		update();
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

	red_hedit = new QLineEdit(this);
    red_hedit->setText("1");
    layout->addWidget(red_hedit);

	green_hedit = new QLineEdit(this);
    green_hedit->setText("0.5");
    layout->addWidget(green_hedit);

	blue_hedit = new QLineEdit(this);
    blue_hedit->setText("1");
    layout->addWidget(blue_hedit);

	alpha_hedit = new QLineEdit(this);
    alpha_hedit->setText("0");
    layout->addWidget(alpha_hedit);

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

	red_vedit = new QLineEdit(this);
    red_vedit->setMaximumWidth(50);
    red_slider = new slider(-4, 4);
	connect(red_slider, &slider::moved, this, [=](float value){
		set_color(value, green, blue, alpha);
	});

	red_layout->addWidget(red_vedit);
	red_layout->addWidget(red_slider);
    layout->addWidget(red_widget);
	//

	// Green
	QWidget *green_widget = new QWidget(this);
    QHBoxLayout *green_layout = new QHBoxLayout(green_widget);
	green_layout->setMargin(0);

    green_vedit = new QLineEdit(this);
    green_vedit->setMaximumWidth(50);
    green_slider = new slider(-4, 4);
	connect(green_slider, &slider::moved, this, [=](float value){
		set_color(red, value, blue, alpha);
	});

	green_layout->addWidget(green_vedit);
	green_layout->addWidget(green_slider);
    layout->addWidget(green_widget);
	//

	// Blue
	QWidget *blue_widget = new QWidget(this);
	QHBoxLayout *blue_layout = new QHBoxLayout(blue_widget);
	blue_layout->setMargin(0);

	blue_vedit = new QLineEdit(this);
	blue_vedit->setMaximumWidth(50);
	blue_slider = new slider(-4, 4);
	connect(blue_slider, &slider::moved, this, [=](float value){
		set_color(red, green, value, alpha);
	});

	blue_layout->addWidget(blue_vedit);
	blue_layout->addWidget(blue_slider);
	layout->addWidget(blue_widget);
	//

	// Alpha
	QWidget *alpha_widget = new QWidget(this);
    QHBoxLayout *alpha_layout = new QHBoxLayout(alpha_widget);
	alpha_layout->setMargin(0);

    alpha_vedit = new QLineEdit(this);
    alpha_vedit->setMaximumWidth(50);
    alpha_slider = new slider(-4, 4);
	connect(alpha_slider, &slider::moved, this, [=](float value){
		set_color(red, green, blue, value);
	});

	alpha_layout->addWidget(alpha_vedit);
	alpha_layout->addWidget(alpha_slider);
    layout->addWidget(alpha_widget);
	//


    return widget;
}
