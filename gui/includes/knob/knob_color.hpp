#ifndef KNOB_SEPARATOR_HPP
#define KNOB_SEPARATOR_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>

// VINACOMP
#include <qt.hpp>

class knob_color : public QWidget
{
private:
    QLabel *label;
    QWidget *_separate_colors_box;
    QWidget *_separate_colors_slider;
    QLineEdit *value;
    QSlider *slider;

    bool is_separate_colors_box = false;
    bool is_separate_colors_slider = false;

    void setup_ui();
    QWidget *separate_colors_box();
    QWidget *separate_colors_slider();

public:
    knob_color(/* args */);
    ~knob_color();
};

#endif //KNOB_SEPARATOR_HPP