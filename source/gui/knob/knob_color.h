#ifndef KNOB_COLOR_HPP
#define KNOB_COLOR_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

// VINACOMP
#include <knob.h>
#include <qt.h>
#include <slider.h>

class knob_color : public knob
{
private:
	QHBoxLayout *layout;

    QWidget *separate_colors_box;
    QWidget *separate_colors_slider;
    QHBoxLayout *separate_colors_box_layout;
    QVBoxLayout *separate_colors_slider_layout;

    QLineEdit *mono_edit;
    slider *mono_slider;

	QPushButton *picker_button;
	QPushButton *picker;
	QPushButton *mono_color_button;
	QPushButton *animation_button;

	QWidget *red_widget;
	QWidget *green_widget;
	QWidget *blue_widget;
	QWidget *alpha_widget;
	QHBoxLayout *red_layout;
	QHBoxLayout *green_layout;
	QHBoxLayout *blue_layout;
	QHBoxLayout *alpha_layout;

	QLineEdit *red_vedit;
	QLineEdit *green_vedit;
	QLineEdit *blue_vedit;
	QLineEdit *alpha_vedit;

	QLineEdit *red_hedit;
	QLineEdit *green_hedit;
	QLineEdit *blue_hedit;
	QLineEdit *alpha_hedit;

	slider *red_slider;
	slider *green_slider;
	slider *blue_slider;
	slider *alpha_slider;

	float red, green, blue, alpha;
	bool mono_color;
    bool sliders_colors;

    void setup_ui();
	void connections();

	void update();
	void toggle_mono_color();
	void toggle_sliders_colors();
	void set_visible_mono_color(bool visible);
	void set_visible_sliders_colors(bool visible);

public:
    knob_color(
        QColor _default_value);
    ~knob_color();

	void set_color(float red, float green, float blue, float alpha = 1);
};

#endif //KNOB_COLOR_HPP
