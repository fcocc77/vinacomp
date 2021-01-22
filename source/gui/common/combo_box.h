#ifndef COMBO_BOX_HPP
#define COMBO_BOX_HPP

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QStyleOption>
#include <QPainter>

#include <util.h>
#include <action.h>

class combo_box : public QWidget
{
	Q_OBJECT
#include <q_object.h>
private:
	QLabel *label;
    QMenu *menu;
    QHBoxLayout *layout;
    QLabel *arrow;

    QList<action *> actions;

    int current_index;

public:
    combo_box(QStringList items = {}, int default_index = 0);
   ~combo_box();

    void add_item(QString name);
    void set_index(int _index, bool emit_signal = true);
    void set_value(QString name);

	int get_index() const;
	QString get_value() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
	void changed(QString name, int index);
};

#endif // COMBO_BOX_HPP
