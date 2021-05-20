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
#include <tools.h>

class menu : public QMenu
{
private:
    button *popup_button;
    QList<action *> actions;

public:
    menu(QWidget *parent, button *_button);
    ~menu();

    void add_action(action *_action);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
};

class nodes_bar : public tools
{
    Q_OBJECT
#include <q_object.h>
private:
    QHBoxLayout *layout;
    maker *_maker;
    nodes_load *nodes;
    QWidget *_node_graph;
    QLineEdit *find_node_edit;
    QHBoxLayout *nodes_layout;

    QList<menu *> py_plugins_menus;

    menu *add_menu(QString group, QString icon_group);

public:
    nodes_bar(QWidget *_node_graph, maker *_maker, nodes_load *_nodes);
    ~nodes_bar();

    void update_py_plugins();
    inline void focus_to_find_edit();

signals:
    void search_changed(QString key);
};

inline void nodes_bar::focus_to_find_edit()
{
    find_node_edit->setFocus();
}

#endif // NODES_BAR_H
