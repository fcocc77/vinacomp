#include <knob_color.h>

knob_color::knob_color(QColor color)
	: red(color.red() / 255.0),
	green(color.green() / 255.0),
	blue(color.blue() / 255.0),
	alpha(color.alpha() / 255.0)
{
    this->setObjectName("knob_color");

    setup_ui();
	set_color(1, 0.2, 0.6);
}

knob_color::~knob_color()
{
}

void knob_color::update()
{
	set_color(red, green, blue, alpha);
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
