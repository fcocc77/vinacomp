#ifndef NODES_BAR_H
#define NODES_BAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QApplication>

#include <qt.h>
#include <maker.h>
#include <util.h>
#include <nodes_load.h>

class menu : public QMenu
{
private:
    QPushButton *popup_button;

public:
    menu(QPushButton *parent) : QMenu(parent), popup_button(parent){};

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

class nodes_bar : public QWidget
{
private:
    QHBoxLayout *layout;
    maker *_maker;
    nodes_load *nodes;

    void setup_ui();
    void add_menu(QString group, QString icon_group);

public:
    nodes_bar(maker *_maker, nodes_load *_nodes);
    ~nodes_bar();
};

#endif // NODES_BAR_H