#ifndef KNOB_COLOR_SLIDER_H
#define KNOB_COLOR_SLIDER_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>

#include <slider.h>

class knob_color_slider : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QLineEdit *edit;
    slider *_slider;
    QLabel *label;

    float value;

public:
    knob_color_slider(float min, float max, float default_value,
                      bool centered_handler, QString label);
    ~knob_color_slider();

    void set_value(float value, bool emmit_signal = true);
    void set_default_value(float _default_value);
    inline float get_value() const;

signals:
    void changed(float value);
};

inline float knob_color_slider::get_value() const
{
    return value;
}

#endif // KNOB_COLOR_SLIDER_H
