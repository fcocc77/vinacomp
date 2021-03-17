#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>
#include <slider.h>

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

#endif // KNOB_INTEGER_HPP
