#ifndef KNOB_CHECK_BOX_HPP
#define KNOB_CHECK_BOX_HPP

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include <knob.h>
#include <util.h>

class knob_check_box : public knob
{
    Q_OBJECT
private:
    QLabel *label_widget;
    QCheckBox *checkbox;

    QString label;
    bool checked;
    bool emmit_signal;

public:
    knob_check_box(QString _label, bool _default_value);
    ~knob_check_box();

    void set_check(bool value, bool emmit_signal = true);
    bool is_checked() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void changed(bool value);
};

#endif // KNOB_CHECK_BOX_HPP
