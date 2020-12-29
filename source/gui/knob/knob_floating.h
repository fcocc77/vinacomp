#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.h>
#include <qt.h>
#include <knob.h>

class knob_floating : public knob
{
private:
	float min;
	float max;
    QHBoxLayout *layout;
    QString label;
    float default_value;
	slider *_slider;
	QLineEdit *value_edit;
	QPushButton *animation_button;
public:
    knob_floating(
        float _default_value);
    ~knob_floating();

	void set_value(float value);
	float get_value() const;
};

#endif //KNOB_FLOATING_HPP
