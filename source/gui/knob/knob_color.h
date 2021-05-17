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
#include <slider.h>

class knob_color : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *layout;

    QWidget *separate_colors_box;
    QWidget *separate_colors_slider;
    QHBoxLayout *separate_colors_box_layout;
    QVBoxLayout *separate_colors_slider_layout;

    QLineEdit *mono_edit;
    slider *mono_slider;

    QPushButton *picker_button;
    QPushButton *palette_button;
    QPushButton *mono_color_button;

    QWidget *red_widget;
    QWidget *green_widget;
    QWidget *blue_widget;
    QWidget *alpha_widget;
    QHBoxLayout *red_layout;
    QHBoxLayout *green_layout;
    QHBoxLayout *blue_layout;
    QHBoxLayout *alpha_layout;

    QLineEdit *red_vedit;
    QLineEdit *green_vedit;
    QLineEdit *blue_vedit;
    QLineEdit *alpha_vedit;

    QLineEdit *red_hedit;
    QLineEdit *green_hedit;
    QLineEdit *blue_hedit;
    QLineEdit *alpha_hedit;

    slider *red_slider;
    slider *green_slider;
    slider *blue_slider;
    slider *alpha_slider;

    float red, green, blue, alpha;
    float default_red, default_green, default_blue, default_alpha;
    bool mono_color;
    bool sliders_colors;

    void setup_ui();
    void connections();

    void update();
    void toggle_mono_color();
    void toggle_sliders_colors();
    void set_visible_mono_color(bool visible);
    void set_visible_sliders_colors(bool visible);
    void init_colors();

    void restore_default() override;
    void restore_param() override;

public:
    knob_color(knob_props, float min = 0, float max = 100, float r = 1,
               float g = 1, float b = 1, float a = 1,
               bool centered_handler = false);
    ~knob_color();

    void set_color(float red, float green, float blue, float alpha = 1,
                   bool set_sliders = true, bool emmit_signal = true);
signals:
    void changed(float r, float g, float b, float a);
};

#endif // KNOB_COLOR_HPP
