#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>
#include <knob.hpp>

class knob_integer : public knob
{
private:
    /* data */
public:
    knob_integer(/* args */);
    ~knob_integer();
};

#endif //KNOB_INTEGER_HPP