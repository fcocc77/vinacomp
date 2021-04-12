#ifndef KNOB_INTD_H
#define KNOB_INTD_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>
#include <slider.h>

class knob_intd : public knob
{
    Q_OBJECT
private:
    QList<QLineEdit *> dimensions_edits;
    QList<int> values;

    void restore_param() override;

public:
    knob_intd(QList<int> default_values = {0, 1});
    ~knob_intd();

    int get_value(int dimension) const;
    QList<int> get_values() const;
    void set_value(int value, int dimension = 0);
    void set_values(QList<int> values, bool emmit_signal = true);
    void emmit_signal();
signals:
    void changed(QList<int> values);
};

#endif // KNOB_INTD_H
