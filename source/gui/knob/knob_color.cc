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

    setup_ui();
	set_color(1, 0.2, 0.6);
}

knob_color::~knob_color()
{
	// falta borrar algunos widgets !
	delete _separate_colors_box;
	delete _separate_colors_slider;

	delete mono_edit;
	delete mono_slider;

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
		_separate_colors_box->setVisible(false);
	}
	else
   	{
		_separate_colors_box->setVisible(!visible);
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
	_separate_colors_slider->setVisible(visible);

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
