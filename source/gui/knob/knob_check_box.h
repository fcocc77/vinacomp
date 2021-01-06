#ifndef KNOB_CHECK_BOX_HPP
#define KNOB_CHECK_BOX_HPP

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>

#include <knob.h>
#include <util.h>

class knob_check_box : public knob
{
private:
    QLabel *label_widget;
    QCheckBox *checkbox;

    QString label;
	bool checked;

public:
    knob_check_box(
        QString _label,
        bool _default_value);
    ~knob_check_box();

    void set_check(bool value);
	bool is_checked() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif //KNOB_CHECK_BOX_HPP
