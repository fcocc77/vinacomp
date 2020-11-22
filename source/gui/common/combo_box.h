#ifndef COMBO_BOX_HPP
#define COMBO_BOX_HPP

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

#include <util.h>
#include <action.h>

class combo_box : public QWidget
{
private:
    QLabel *label;
    QMenu *menu;
    QHBoxLayout *layout;
    QLabel *arrow;

    QList<action *> actions;

    int current_index;

public:
    combo_box(/* args */);
    ~combo_box();

    void add_item(QString name);
    void add_items(QStringList list);
    void set_index(int _index);
    void set_value(QString name);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // COMBO_BOX_HPP