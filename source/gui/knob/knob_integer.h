#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <knob_slider.h>

class knob_integer : public knob_slider
{
    Q_OBJECT
private:

public:
    knob_integer(int min = 0, int max = 100, int default_value = 0,
                 bool two_dimensional = false);
    ~knob_integer();
};

#endif // KNOB_INTEGER_HPP
