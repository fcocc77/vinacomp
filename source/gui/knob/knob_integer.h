#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.h>
#include <qt.h>
#include <knob.h>

class knob_integer : public knob
{
	Q_OBJECT
private:
	QLineEdit *value_edit;
	slider *_slider;
public:
    knob_integer(int min, int max, int default_value = 0);
    ~knob_integer();

	void set_value(int value);
signals:
	void changed(int value);
};

#endif //KNOB_INTEGER_HPP
