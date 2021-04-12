#ifndef KNOB_BUTTON_HPP
#define KNOB_BUTTON_HPP

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include <knob.h>

class knob_button : public knob
{
    Q_OBJECT
private:
    QPushButton *button;
    void restore_param() override;

public:
    knob_button();
    ~knob_button();
};

#endif // KNOB_BUTTON_HPP
