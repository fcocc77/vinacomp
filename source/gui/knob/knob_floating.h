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
	Q_OBJECT
private:
    QHBoxLayout *layout;
    QString label;
	slider *_slider;
	QLineEdit *value_edit;
public:
    knob_floating(
		float min,
		float max,
        float default_value = 1
	);
    ~knob_floating();

	void set_value(float value);
	float get_value() const;
signals:
	void changed(float value);
};

#endif //KNOB_FLOATING_HPP
