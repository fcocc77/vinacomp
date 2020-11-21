#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>
#include <knob.hpp>

class knob_floating : public knob
{
private:
    QString label;
    float default_value;

public:
    knob_floating(
        float _default_value);
    ~knob_floating();
};

#endif //KNOB_FLOATING_HPP