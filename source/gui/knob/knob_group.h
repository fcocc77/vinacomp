#ifndef KNOB_GROUP_HPP
#define KNOB_GROUP_HPP

#include <QWidget>
#include <QStyle>
#include <QPushButton>

#include <knob.h>
#include <util.h>
#include <qt.h>

class knob_group : public knob
{
private:
	QLabel *label_widget;
	int knobs_included;
	bool open;
	QPushButton *arrow_button;

	void set_open(bool _open);
	QList <QWidget *> get_knobs_included();
public:
    knob_group(QString label, int knobs_included, bool open_group);
    ~knob_group();

	void update();

protected:
    void mousePressEvent(QMouseEvent *event) override;

};

#endif //KNOB_GROUP_HPP
