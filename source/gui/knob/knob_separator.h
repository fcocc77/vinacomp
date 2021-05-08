#ifndef KNOB_SEPARATOR_HPP
#define KNOB_SEPARATOR_HPP

#include <QHBoxLayout>
#include <QWidget>

#include <knob.h>

class knob_separator : public knob
{
public:
    knob_separator(knob_props props);
    ~knob_separator();
};

#endif // KNOB_SEPARATOR_HPP
