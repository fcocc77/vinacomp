#ifndef KNOB_HPP
#define KNOB_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include <qt.h>

class knob : public QWidget
{
private:
	QHBoxLayout *knob_layout;
	QPushButton *animation_button;

protected:
	QList <QWidget*> *viewers_gl;
	QString node_name;
	QString node_type;
	QString param_name;

public:
    knob();
    ~knob();

    void set_init_space(int space, QString label = "");
	void set_knob_layout(QHBoxLayout *layout);
	void set_animatable(bool _animatable = true);
	void set_visible(bool visible);
	void set_viewers_gl(QList <QWidget *> *viewers_gl);
	void set_names(QString node_name, QString _node_type, QString _param_name);
	QString get_full_name() const;
	QString get_node_type() const;

    QLabel *label_widget;
    QWidget *init_space;

	int icon_size;
};

#endif //KNOB_HPP
