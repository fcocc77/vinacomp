#ifndef KNOB_FLOATING_HPP
#define KNOB_FLOATING_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <button.h>
#include <knob.h>
#include <line_edit.h>
#include <qt.h>
#include <slider.h>

class knob_floating : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QString label;
    slider *_slider;
    line_edit *value_1_edit;
    line_edit *value_2_edit;
    bool dimensions;
    bool two_dimensional;
    button *show_dimensions;
    bool emmit_signal;
    QWidget *empty_widget;

    pair<float, float> values;

    void setup_ui(float min, float max, float default_value);

    void separate_dimensions(bool separate);
    void set_value_internal(float value, int dimension);

    void restore_param() override;
    void set_animated(bool animated) override;
    void update_animated() override;

public:
    knob_floating(float min = 0, float max = 100, float default_value = 1,
                  bool two_dimensional = false);
    ~knob_floating();

    void set_value(float value, int dimension = 0, bool _emmit_signal = true);
    void set_values(pair<float, float> values, bool _emmit_signal = true);
    float get_value(int dimension = 0) const;
signals:
    void changed(float value1, float value2 = 0);
};

#endif // KNOB_FLOATING_HPP
