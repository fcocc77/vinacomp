#ifndef KNOB_POSITION_HPP
#define KNOB_POSITION_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.hpp>
#include <qt.hpp>
#include <knob.hpp>

class knob_position : public knob
{
private:
    /* data */
public:
    knob_position(/* args */);
    ~knob_position();
};

#endif //KNOB_POSITION_HPP