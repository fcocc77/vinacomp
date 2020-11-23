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
#include <util.h>

class tool_bar : public QWidget
{
private:
    QVBoxLayout *layout;

    void setup_ui();

public:
    tool_bar();
    ~tool_bar();
};

#endif // TOOL_BAR_HPP