#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>

#include <action.h>
#include <qt.h>
#include <util.h>

class tools : public QWidget
{
private:
    QBoxLayout *layout;
    int icon_size;
    QList<action *> actions;
    bool vertical;
    bool uncheck_all;
    bool one_check_at_time;

public:
    tools(int icon_size = 22, bool _vertical = false);
    ~tools();

    void add_action(action *_action);
    void add_separator();
    void add_stretch();
    void add_widget(QWidget *widget);
    void set_checked_all(bool checked);
    void allow_one_check_at_time();

    QBoxLayout *get_layout() const;
};

#endif // TOOLS_HPP
