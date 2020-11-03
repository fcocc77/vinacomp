#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>

class knob_floating : public QWidget
{
private:
    QString label;
    float default_value;

public:
    knob_floating(
        QString _label,
        float _default_value);
    ~knob_floating();
};

#endif //KNOB_FLOATING_HPP