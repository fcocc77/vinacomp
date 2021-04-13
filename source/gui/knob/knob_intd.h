#ifndef KNOB_INTD_H
#define KNOB_INTD_H

#include <knob_dimensional.h>

class knob_intd : public knob_dimensional
{
    Q_OBJECT
private:
    QList<float> int_to_float(QList<int> defaults);
    void changed_values(QList<float> values) override;

public:
    knob_intd(int dimensions_count = 2, QList<int> defaults = {0, 1});
    ~knob_intd();

signals:
    void changed(QList<int> values);
};

#endif // KNOB_INTD_H
