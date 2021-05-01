#ifndef KNOB_INTEGER_HPP
#define KNOB_INTEGER_HPP

#include <knob_slider.h>

class knob_integer : public knob_slider
{
    Q_OBJECT
private:
public:
    knob_integer(int min = 0, int max = 100, int default_value = 0,
                 bool two_dimensional = false, bool centered_handler = false);
    ~knob_integer();

    inline void set_value(int value, int dimension = 0,
                          bool _emmit_signal = true);
    inline void set_values(pair<int, int> values, bool _emmit_signal = true);
    inline int get_value(int dimension = 0) const;
};

inline void knob_integer::set_value(int value, int dimension,
                                    bool _emmit_signal)
{
    knob_slider::set_value(float(value));
}

inline void knob_integer::set_values(pair<int, int> values, bool _emmit_signal)
{
    knob_slider::set_values({float(values.first), float(values.second)},
                            _emmit_signal);
}

inline int knob_integer::get_value(int dimension) const
{
    return knob_slider::get_value(dimension);
}

#endif // KNOB_INTEGER_HPP
