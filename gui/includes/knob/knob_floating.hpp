#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>
#include <init_space.hpp>

class knob_floating : public QWidget
{
private:
    QString label;
    float default_value;

    int init_space_width;

public:
    knob_floating(
        int _init_space_width,
        QString _label,
        float _default_value);
    ~knob_floating();
};

#endif //KNOB_FLOATING_HPP