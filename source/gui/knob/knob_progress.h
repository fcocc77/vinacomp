#ifndef KNOB_PROGRESS_H
#define KNOB_PROGRESS_H

#include <QHBoxLayout>
#include <QProgressBar>
#include <QWidget>

#include <knob.h>

class knob_progress : public knob
{
    Q_OBJECT
private:
    QProgressBar *bar;
    void restore_param() override;

public:
    knob_progress();
    ~knob_progress();

    inline void set_value(int value);
};

inline void knob_progress::set_value(int value)
{
    bar->setValue(value);
}

#endif // KNOB_PROGRESS_H
