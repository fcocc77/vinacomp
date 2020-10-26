#ifndef TOOL_BAR_HPP
#define TOOL_BAR_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include <qt.hpp>

class tool_bar : public QWidget
{
private:
    void setup_ui();

    QVBoxLayout *layout;

public:
    tool_bar(/* args */);
    ~tool_bar();
};

#endif // TOOL_BAR_HPP