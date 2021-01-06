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
private:
	QLineEdit *value_edit;
public:
    knob_integer(/* args */);
    ~knob_integer();
};

#endif //KNOB_INTEGER_HPP
