#ifndef KNOB_COLOR_SLIDER_H
#define KNOB_COLOR_SLIDER_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>

#include <slider.h>

class colored_slider : public QWidget
{
private:
    slider *_slider;
    bool hue, sat, level;
    QColor color;

    float hue_value, sat_value;

    void paintEvent(QPaintEvent *event) override;
    void update_handler_color(float _hue, float _sat);

public:
    colored_slider(slider *_slider, QString hsl_type);
    void set_hue_color(float hue);
    void set_value(float value);
};

class knob_color_slider : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QLineEdit *edit;
    slider *_slider;
    QLabel *label;
    colored_slider *_colored_slider;

    float value;

public:
    knob_color_slider(float min, float max, float default_value,
                      bool centered_handler, QString label = "");
    ~knob_color_slider();

    void set_value(float value, bool emmit_signal = true);
    void set_default_value(float _default_value);
    inline float get_value() const;
    void set_hue_color(float hue);

    void set_colored_slider(QString hsl_type);

signals:
    void changed(float value);
};

inline float knob_color_slider::get_value() const
{
    return value;
}

#endif // KNOB_COLOR_SLIDER_H
