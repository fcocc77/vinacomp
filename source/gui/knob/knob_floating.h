#ifndef KNOB_FLOATING_H
#define KNOB_FLOATING_H

#include <knob_slider.h>

class knob_floating : public knob_slider
{
    Q_OBJECT
private:

public:
    knob_floating(float min = 0, float max = 100, float default_value = 0,
                  bool two_dimensional = false, bool centered_handler = false);
    ~knob_floating();
};

#endif // KNOB_FLOATING_H
