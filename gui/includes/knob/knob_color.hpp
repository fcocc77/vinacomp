#ifndef KNOB_COLOR_HPP
#define KNOB_COLOR_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

// VINACOMP
#include <qt.hpp>
#include <slider.hpp>
#include <init_space.hpp>

class knob_color : public QWidget
{
private:
    QWidget *_separate_colors_box;
    QWidget *_separate_colors_slider;
    QLineEdit *value;
    slider *_slider;


    int init_space_width;
    QString label;
    QColor default_value;

    bool is_separate_colors_box = false;
    bool is_separate_colors_slider = false;

    void setup_ui();
    QWidget *separate_colors_box();
    QWidget *separate_colors_slider();

public:
    knob_color(
        int _init_space_width,
        QString _label,
        QColor _default_value);
    ~knob_color();
};

#endif //KNOB_COLOR_HPP