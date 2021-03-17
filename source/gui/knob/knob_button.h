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
    /* data */
public:
    knob_button( QString label );
    ~knob_button();
signals:
    void clicked();
};

#endif // KNOB_BUTTON_HPP
