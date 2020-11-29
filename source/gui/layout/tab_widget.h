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

    void delete_tab(tab *_tab);

public:
    tab_widget();
    ~tab_widget();

    void add_tab(QWidget *tab, QString label);
    int get_index_by_name(QString name) const;
    void set_tab(QString name);
    void set_index(int index);
    void close_tab(QString name);
    QPushButton *add_cornel_button(QString icon);
    int get_current_index() const;
    void clear();
};

#endif // TAB_WIDGET_H