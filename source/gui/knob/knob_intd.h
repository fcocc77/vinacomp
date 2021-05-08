#ifndef KNOB_INTD_H
#define KNOB_INTD_H

#include <knob_dimensional.h>

class knob_intd : public knob_dimensional
{
    Q_OBJECT
private:
    QList<float> int_to_float(QList<int> defaults) const;
    QList<int> float_to_int(QList<float> values) const;
    void changed_values(QList<float> values) override;

public:
    knob_intd(knob_props props, int dimensions_count = 2,
              QList<int> defaults = {0, 1});
    ~knob_intd();

    inline void set_values(QList<int> values, bool emmit_signal = true);
    inline int get_value(int dimension) const;
    inline QList<int> get_values() const;

signals:
    void changed(QList<int> values);
};

inline void knob_intd::set_values(QList<int> values, bool emmit_signal)
{
    // convierte los valores a flotante ya
    // que 'knob_dimensional' solo funciona en flotante.
    knob_dimensional::set_values(int_to_float(values), emmit_signal);
}

inline int knob_intd::get_value(int dimension) const
{
    return knob_dimensional::get_value(dimension);
}

inline QList<int> knob_intd::get_values() const
{
    return float_to_int(knob_dimensional::get_values());
}

#endif // KNOB_INTD_H
