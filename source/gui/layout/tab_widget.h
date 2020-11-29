#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <util.h>
#include <qt.h>
#include <tab.h>

class tab_widget : public QWidget
{
private:
    QVBoxLayout *layout;

    QHBoxLayout *tab_bar_layout;
    QWidget *tab_bar;
    QWidget *widget_section;
    QVBoxLayout *widget_section_layout;

    int current_index;
    QPushButton *cornel_button;

    QList<tab *> tabs;

public:
    tab_widget();
    ~tab_widget();

    void add_tab(QWidget *tab, QString label);
    void set_tab(QString name);
    void set_index(int index);
    QPushButton *add_cornel_button(QString icon);
};

#endif // TAB_WIDGET_H