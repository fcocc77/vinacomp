#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>
#include <slider.h>

class knob_floating : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QString label;
    slider *_slider;
    QLineEdit *value_edit;
    bool emmit_signal;

public:
    knob_floating(float min, float max, float default_value = 1);
    ~knob_floating();

    void set_value(float value, bool _emmit_signal = true);
    float get_value() const;
signals:
    void changed(float value);
};

#endif // KNOB_FLOATING_HPP
