#ifndef KNOB_GROUP_HPP
#define KNOB_GROUP_HPP

#include <QPushButton>
#include <QStyle>
#include <QWidget>

#include <knob.h>
#include <qt.h>
#include <util.h>

class knob_group : public knob
{
private:
    QLabel *label_widget;
    int knobs_included;
    bool open;
    QPushButton *arrow_button;

    void set_open(bool _open);
    QList<QWidget *> get_knobs_included();
    void restore_param() override;

public:
    knob_group(knob_props props, QString label = "", int knobs_included = 0,
               bool open_group = true);
    ~knob_group();

    void update();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // KNOB_GROUP_HPP
