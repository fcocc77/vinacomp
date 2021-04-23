#ifndef NODES_BAR_H
#define NODES_BAR_H

#include <QApplication>
#include <QHBoxLayout>
#include <QMenu>
#include <QPushButton>
#include <QWidget>

#include <button.h>
#include <maker.h>
#include <nodes_load.h>

class menu : public QMenu
{
private:
    button *popup_button;

public:
    menu(button *parent)
        : QMenu(parent)
        , popup_button(parent){};

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
};

class nodes_bar : public QWidget
{
    Q_OBJECT
#include <q_object.h>
private:
    QHBoxLayout *layout;
    maker *_maker;
    nodes_load *nodes;
    QWidget *parent;
    QLineEdit *find_node_edit;

    void setup_ui();
    void add_menu(QString group, QString icon_group);

public:
    nodes_bar(QWidget *parent, maker *_maker, nodes_load *_nodes);
    ~nodes_bar();

    inline void focus_to_find_edit();
signals:
    void search_changed(QString key);
};

inline void nodes_bar::focus_to_find_edit()
{
    find_node_edit->setFocus();
}

#endif // NODES_BAR_H
