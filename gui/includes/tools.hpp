#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include <action.hpp>
#include <util.hpp>
#include <qt.hpp>

class tools : public QWidget
{
private:
    QHBoxLayout *layout;

public:
    tools(/* args */);
    ~tools();

    void add_action(action *_action);
    void add_separator();
    void add_stretch();
};

#endif // TOOLS_HPP