#ifndef KNOB_SLIDER_H
#define KNOB_SLIDER_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <button.h>
#include <knob.h>
#include <line_edit.h>
#include <qt.h>
#include <slider.h>

class knob_slider : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QString label;
    slider *_slider;
    line_edit *value_1_edit;
    line_edit *value_2_edit;
    bool dimensions;
    bool bidimensional;
    button *show_dimensions;
    QWidget *empty_widget;
    float default_value;

    pair<float, float> values;

    void separate_dimensions(bool separate);
    void to_emmit_signal();
    void update_linked_knobs();

    void restore_param() override;
    void set_animated(bool animated, int dimension = -1) override;
    void set_disable(bool disable) override;
    void set_error(bool error) override;
    void set_has_expression(bool expression) override;
    void update_animated() override;
    void restore_default() override;

public:
    knob_slider(knob_props props, float min = 0, float max = 100,
                float default_value = 1, bool bidimensional = false,
                bool floating = true, bool centered_handler = false);
    ~knob_slider();

    void set_value(float value, int dimension = 0, bool _emmit_signal = true);
    void set_values(pair<float, float> values, bool _emmit_signal = true);
    float get_value(int dimension = 0) const;
signals:
    void changed(float value1, float value2 = 0);
};

#endif // KNOB_SLIDER_H
