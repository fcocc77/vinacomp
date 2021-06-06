#ifndef KNOB_DIMENSIONAL_H
#define KNOB_DIMENSIONAL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>
#include <slider.h>

class knob_dimensional : public knob
{
    Q_OBJECT
private:
    QList<QLineEdit *> dimensions_edits;
    QList<float> default_values;
    QList<float> values;

    bool floating;
    int dimensions_count;

    void restore_default() override;
    void restore_param() override;

    virtual void changed_values(QList<float> values);

public:
    knob_dimensional(knob_props props, int dimensions_count = 2,
                     QList<float> default_values = {0, 1},
                     bool floating = true);
    ~knob_dimensional();

    float get_value(int dimension) const;
    QList<float> get_values() const;
    void set_value(float value, int dimension = 0);
    void set_values(QList<float> values, bool emmit_signal = true);
    void emmit_signal();
    inline int get_dimensions_count() const;
};

inline int knob_dimensional::get_dimensions_count() const
{
    return dimensions_count;
}

#endif // KNOB_DIMENSIONAL_H
