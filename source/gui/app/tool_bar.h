#ifndef TOOL_BAR_HPP
#define TOOL_BAR_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QJsonObject>
#include <QLabel>
#include <QApplication>

#include <qt.h>
#include <maker.h>
#include <util.h>

class menu : public QMenu
{
private:
    QWidget *button;

public:
    menu(QWidget *_button) : QMenu(_button), button(_button){};
    ~menu(){};

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
};

class tool_bar : public QWidget
{
private:
    QVBoxLayout *layout;
    maker *_maker;
    bool open_menus;
    QMap<QString, menu *> menus;

    void setup_ui();
    void add_menu(QString group, QString icon_group);

public:
    tool_bar(maker *_maker);
    ~tool_bar();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};

#endif // TOOL_BAR_HPP