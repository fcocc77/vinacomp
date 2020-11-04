#ifndef KNOB_CHECK_BOX_HPP
#define KNOB_CHECK_BOX_HPP

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>

#include <init_space.hpp>

class knob_check_box : public QWidget
{
private:
    QLabel *label_widget;
    QCheckBox *checkbox;
    int init_space_width;

    QString label;
    bool default_value;

public:
    knob_check_box(
        int _init_space_width,
        QString _label,
        bool _default_value);
    ~knob_check_box();
    void set_check(bool value);
};

#endif //KNOB_CHECK_BOX_HPP