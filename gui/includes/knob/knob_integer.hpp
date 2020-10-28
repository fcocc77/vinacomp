#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>

class knob_integer : public QWidget
{
private:
    /* data */
public:
    knob_integer(/* args */);
    ~knob_integer();
};

#endif //KNOB_INTEGER_HPP