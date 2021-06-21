#ifndef KNOB_COLOR_HPP
#define KNOB_COLOR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

// VINACOMP
#include <knob.h>
#include <qt.h>
#include <color_picker.h>
#include <knob_color_slider.h>
#include <button.h>
#include <line_edit.h>

class knob_color : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *main_layout;
    QVBoxLayout *content_layout;
    QHBoxLayout *top_layout;

    QWidget *content_widget, *top_widget;

    QWidget *separate_colors_box, *rgba_box, *hsl_box;
    QHBoxLayout *separate_colors_box_layout;
    QVBoxLayout *rgba_box_layout, *hsl_box_layout;

    color_picker *_color_picker;

    button *color_sample_button;
    button *palette_button;
    button *mono_color_button;
    button *rgb_button;
    button *hsl_button;
    button *picker_button;

    knob_color_slider *red_slider, *green_slider, *blue_slider, *alpha_slider;
    knob_color_slider *mono_slider;
    knob_color_slider *hue_slider, *sat_slider, *level_slider;
    line_edit *red_hedit, *green_hedit, *blue_hedit, *alpha_hedit;

    float red, green, blue, alpha;
    float default_red, default_green, default_blue, default_alpha;

    bool mono_color;
    bool advanced_options;
    bool hsl_option;
    bool rgb_option;
    bool hue_option;
    bool color_picker_option;

    void setup_ui();
    void connections();
    void update();

    void toggle_mono_color();
    void toogle_advanced_options();
    void toogle_rgb_option();
    void toogle_hsl_option();
    void toogle_color_picker_option();

    void set_visible_mono_color(bool visible, bool emmit_signal = true);
    void set_visible_rgba_box(bool visible);

    void restore_default() override;
    void restore_param() override;
    void set_animated(bool animated, int dimension = -1) override;

    void update_hsl_sliders();
    void update_rgb_sliders();
    void update_color_picker();

public:
    knob_color(knob_props, float min = 0, float max = 100, float r = 1,
               float g = 1, float b = 1, float a = 1,
               bool centered_handler = false);
    ~knob_color();

    static QColor qcolor_from_rgb(float r, float g, float b, float a = 0);
    static void rgb_from_qcolor(QColor color, float *r, float *g, float *b,
                                float *a = nullptr);

    void set_init_color(float red, float green, float blue, float alpha = 1);

    void set_color(float red, float green, float blue, float alpha = 1,
                   bool emmit_signal = true, bool update_sliders = true);

    void set_color(QColor color, bool emmit_signal = true,
                   bool update_sliders = true);

    void set_hsl(float hue, float sat, float level, bool emmit_signal = true,
                 bool update_sliders = true);

    void get_color(float *r, float *g, float *b, float *a = 0) const;

    void disable_alpha();
signals:
    void changed(float r, float g, float b, float a);
};

#endif // KNOB_COLOR_HPP
