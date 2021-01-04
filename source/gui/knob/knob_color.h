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
    QWidget *_separate_colors_box;
    QWidget *_separate_colors_slider;
    QLineEdit *value_edit;
    slider *_slider;

	QPushButton *picker_button;
	float red, green, blue, alpha;

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

    bool is_separate_colors_box = false;
    bool is_separate_colors_slider = false;

    void setup_ui();
    QWidget *separate_colors_box();
    QWidget *separate_colors_slider();

	void update();

public:
    knob_color(
        QColor _default_value);
    ~knob_color();

	void set_color(float red, float green, float blue, float alpha = 1);
};

#endif //KNOB_COLOR_HPP
