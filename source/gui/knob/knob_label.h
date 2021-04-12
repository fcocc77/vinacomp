#ifndef KNOB_LABEL_HPP
#define KNOB_LABEL_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include <knob.h>

class knob_label : public knob
{
private:
    QLabel *label_widget;
    void restore_param() override;
public:
    knob_label();
    ~knob_label();
};

#endif // KNOB_LABEL_HPP
