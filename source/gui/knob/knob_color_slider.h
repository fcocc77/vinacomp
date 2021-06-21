#ifndef KNOB_COLOR_SLIDER_H
#define KNOB_COLOR_SLIDER_H

#include <QWidget>
#include <QLineEdit>
#include <number_box.h>
#include <QHBoxLayout>
#include <QMenu>

#include <slider.h>

class colored_slider : public QWidget
{
private:
    slider *_slider;
    bool hue, sat, level;
    QColor color;

    float hue_value, sat_value, level_value;

    void paintEvent(QPaintEvent *event) override;
    void update_handler_color(float _hue, float _sat, float _level);

public:
    colored_slider(slider *_slider, QString hsl_type);
    void set_hsl(float hue, float sat, float level);
    void set_value(float value);
};

class knob_color_slider : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    number_box *edit;
    slider *_slider;
    QLabel *label;
    colored_slider *_colored_slider;

    float value;

public:
    knob_color_slider(float min, float max, float default_value,
                      bool centered_handler, QString label,
                      QWidget *knob = nullptr, int dimension = -1,
                      QMenu *curve_menu = nullptr);
    ~knob_color_slider();

    void set_value(float value, bool emmit_signal = true);
    void set_default_value(float _default_value);
    inline float get_value() const;
    void set_hsl(float hue, float sat, float level);

    void set_colored_slider(QString hsl_type);
    inline number_box *get_number_box() const;

signals:
    void changed(float value);
};

inline number_box *knob_color_slider::get_number_box() const
{
    return edit;
}

inline float knob_color_slider::get_value() const
{
    return value;
}

#endif // KNOB_COLOR_SLIDER_H
