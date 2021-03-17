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
    QList<pair<QString, QVariant>> items;
    int current_index;

    void add_item( pair<QString, QVariant> item );

public:
    combo_box( QList<pair<QString, QVariant>> items = {}, int default_index = 0,
               QWidget *_parent = nullptr );
    ~combo_box();

    void set_index( int _index, bool emit_signal = true );
    void set_value( QVariant value );
    void add_shortcut( int index, QString key );

    int get_index() const;
    QVariant get_value() const;

protected:
    void mousePressEvent( QMouseEvent *event ) override;

signals:
    void changed( QVariant value, int index );
};

#endif // COMBO_BOX_HPP
