#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <button.h>
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
    QLineEdit *value_1_edit;
    QLineEdit *value_2_edit;
    bool dimensions;
    bool two_dimensional;
    button *show_dimensions;
    bool emmit_signal;
    QWidget *empty_widget;

    pair<float, float> values;

    void separate_dimensions(bool separate);
    void set_value_internal(float value, int dimension);

    void set_animated(bool animated) override;
    void update_animated() override;

public:
    knob_floating(float min, float max, float default_value = 1,
                  bool two_dimensional = false);
    ~knob_floating();

    void set_value(float value, int dimension = 0, bool _emmit_signal = true);
    void set_values(pair<float, float> values, bool _emmit_signal = true);
    float get_value(int dimension = 0) const;
signals:
    void changed(float value1, float value2 = 0);
};

#endif // KNOB_FLOATING_HPP
