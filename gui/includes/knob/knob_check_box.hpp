#ifndef KNOB_CHECK_BOX_HPP
#define KNOB_CHECK_BOX_HPP

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>


class knob_check_box : public QWidget
{
private:
    QLabel *label;
    QCheckBox *checkbox;

public:
    knob_check_box(/* args */);
    ~knob_check_box();
};

#endif //KNOB_CHECK_BOX_HPP