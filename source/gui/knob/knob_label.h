#ifndef KNOB_LABEL_HPP
#define KNOB_LABEL_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

#include <knob.h>

class knob_label : public knob
{
private:
    /* data */
public:
    knob_label(QString label);
    ~knob_label();
};

#endif //KNOB_LABEL_HPP
