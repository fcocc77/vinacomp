#ifndef TOOL_BAR_HPP
#define TOOL_BAR_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QJsonObject>

#include <qt.h>
#include <maker.h>

class tool_bar : public QWidget
{
private:
    QVBoxLayout *layout;
    maker *_maker;

    void setup_ui();
    void add_menu(QString group, QString icon_group);

public:
    tool_bar(maker *_maker);
    ~tool_bar();
};

#endif // TOOL_BAR_HPP