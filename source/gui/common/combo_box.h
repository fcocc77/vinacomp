#ifndef COMBO_BOX_HPP
#define COMBO_BOX_HPP

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>

#include <action.h>
#include <util.h>

struct combo_box_item
{
    QString label;
    QVariant value;
    bool button;
    QString icon_name;
};

class combo_box : public QWidget
{
    Q_OBJECT
#include <q_object.h>
private:
    QLabel *label;
    QMenu *menu;
    QHBoxLayout *layout;
    QLabel *arrow;
    QWidget *parent;

    QList<action *> actions;
    QList<combo_box_item> items;
    int current_index;

public:
    combo_box(QList<combo_box_item> items = {}, int default_index = 0,
              QWidget *_parent = nullptr);
    ~combo_box();

    void set_index(int _index, bool emit_signal = true);
    void set_value(QVariant value);
    void add_shortcut(int index, QString key);
    int add_item(combo_box_item item);
    void clear();

    inline action *get_action(int index) const;
    inline int get_index() const;
    inline QVariant get_value() const;
    inline int count() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void changed(QVariant value, int index);
    void pre_open();
};

inline int combo_box::count() const
{
    return items.count();
}

inline action *combo_box::get_action(int index) const
{
    return actions.value(index);
}

inline int combo_box::get_index() const
{
    return current_index;
}

inline QVariant combo_box::get_value() const
{
    return items.value(current_index).value;
}

#endif // COMBO_BOX_HPP
