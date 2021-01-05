#include <knob_color.h>

knob_color::knob_color(QColor color)
	: red(color.red() / 255.0),
	green(color.green() / 255.0),
	blue(color.blue() / 255.0),
	alpha(color.alpha() / 255.0),
	mono_color(false),
	sliders_colors(false)
{
    this->setObjectName("knob_color");

    layout = new QHBoxLayout(this);

	separate_colors_box = new QWidget(this);
	separate_colors_slider = new QWidget(this);
	separate_colors_box_layout = new QHBoxLayout(separate_colors_box);
	separate_colors_slider_layout = new QVBoxLayout(separate_colors_slider);

    mono_edit = new QLineEdit(this);
    mono_slider = new slider(-1, 1);

	red_widget = new QWidget(this);
	green_widget = new QWidget(this);
	blue_widget = new QWidget(this);
	alpha_widget = new QWidget(this);
	red_layout = new QHBoxLayout(red_widget);
	green_layout = new QHBoxLayout(green_widget);
	blue_layout = new QHBoxLayout(blue_widget);
	alpha_layout = new QHBoxLayout(alpha_widget);

	red_hedit = new QLineEdit(this);
	green_hedit = new QLineEdit(this);
	blue_hedit = new QLineEdit(this);
	alpha_hedit = new QLineEdit(this);

	red_vedit = new QLineEdit(this);
	green_vedit = new QLineEdit(this);
	blue_vedit = new QLineEdit(this);
	alpha_vedit = new QLineEdit(this);

	red_slider = new slider(-1, 1);
	green_slider = new slider(-1, 1);
	blue_slider = new slider(-1, 1);
	alpha_slider = new slider(-1, 1);

	picker_button = new QPushButton(this);
    picker = new QPushButton(this);
    mono_color_button = new QPushButton(this);
    animation_button = new QPushButton(this);

	connections();
    setup_ui();

	set_color(1, 0.2, 0.6);
}

knob_color::~knob_color()
{
	delete layout;

	delete separate_colors_box;
	delete separate_colors_slider;
	delete separate_colors_box_layout;
	delete separate_colors_slider_layout;

	delete mono_edit;
	delete mono_slider;

	delete red_widget;
	delete green_widget;
	delete blue_widget;
	delete alpha_widget;
	delete red_layout;
	delete green_layout;
	delete blue_layout;
	delete alpha_layout;

	delete red_vedit;
	delete green_vedit;
	delete blue_vedit;
	delete alpha_vedit;

	delete red_hedit;
	delete green_hedit;
	delete blue_hedit;
	delete alpha_hedit;

	delete red_slider;
	delete green_slider;
	delete blue_slider;
	delete alpha_slider;

	delete picker_button;
	delete picker;
	delete mono_color_button;
	delete animation_button;
}

void knob_color::update()
{
	set_color(red, green, blue, alpha);
}

void knob_color::set_visible_mono_color(bool visible)
{
	if (sliders_colors)
	{
		visible = false;
		separate_colors_box->setVisible(false);
	}
	else
   	{
		separate_colors_box->setVisible(!visible);
	}

	mono_slider->setVisible(visible);
	mono_edit->setVisible(visible);

	if (visible)
	{
		// promedia todos los colores para el mono color
		float color = (red + green + blue) / 3.0;

		mono_edit->setText(QString::number(color));
		mono_slider->set_value(color);
		
		set_color(color, color, color, color);
	}

	update();
}

void knob_color::toggle_mono_color()
{
	mono_color = !mono_color;
	set_visible_mono_color(!mono_color);
}

void knob_color::set_visible_sliders_colors(bool visible)
{
	separate_colors_slider->setVisible(visible);

	// al activar el cuadro se colors sliders se desabilita el mono color
	mono_color = !visible;
	set_visible_mono_color(false);
	//

	update();
}

void knob_color::toggle_sliders_colors()
{
	sliders_colors = !sliders_colors;
	set_visible_sliders_colors(sliders_colors);
}

void knob_color::set_color(float _red, float _green, float _blue, float _alpha)
{
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;

	red_vedit->setText(QString::number(red));
	red_hedit->setText(QString::number(red));
	red_slider->set_value(red);

	green_vedit->setText(QString::number(green));
	green_hedit->setText(QString::number(green));
	green_slider->set_value(green);

	blue_vedit->setText(QString::number(blue));
	blue_hedit->setText(QString::number(blue));
	blue_slider->set_value(blue);

	alpha_vedit->setText(QString::number(alpha));
	alpha_hedit->setText(QString::number(alpha));
	alpha_slider->set_value(alpha);

	if (red < 0) _red = 0;
	if (red > 1) _red = 1;

	if (green < 0) _green = 0;
	if (green > 1) _green = 1;

	if (blue < 0) _blue = 0;
	if (blue > 1) _blue = 1;

	// Cambia el color del boton de picker
	QColor color = {_red * 255, _green * 255, _blue * 255};
	QPalette palette = picker_button->palette();
	palette.setColor(QPalette::Button, color);
	picker_button->setAutoFillBackground(true);
	picker_button->setPalette(palette);
	picker_button->update();
}
