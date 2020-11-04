#ifndef KNOB_CHOICE_HPP
#define KNOB_CHOICE_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <combo_box.hpp>
#include <knob.hpp>

class knob_choice : public knob
{
private:
    /* data */
public:
    knob_choice(/* args */);
    ~knob_choice();
};

#endif //KNOB_CHOICE_HPP