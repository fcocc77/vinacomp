#ifndef KNOB_FLOATD_H
#define KNOB_FLOATD_H

#include <knob_dimensional.h>

class knob_floatd : public knob_dimensional
{
    Q_OBJECT
private:
    void changed_values(QList<float> values) override;

public:
    knob_floatd(QList<float> default_values = {0, 1});
    ~knob_floatd();

signals:
    void changed(QList<float> values);
};

#endif // KNOB_FLOATD_H
