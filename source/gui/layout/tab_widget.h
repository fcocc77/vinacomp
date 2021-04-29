#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <qt.h>
#include <tab.h>
#include <util.h>

class tab_widget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *layout;

    QHBoxLayout *tab_bar_layout;
    QWidget *tab_bar;
    QWidget *widget_section;
    QVBoxLayout *widget_section_layout;

    int current_index;
    QPushButton *cornel_button;
    const bool has_close_button;

    QList<tab *> tabs;

    void delete_tab(tab *_tab);

public:
    tab_widget(bool _has_close_button = false);
    ~tab_widget();

    void add_tab(QWidget *tab, QString label);
    void remove_tab(QString name);
    int get_index_by_name(QString name) const;
    void set_tab(QString name);
    void set_index(int index);
    void close_tab(QString name);
    QPushButton *add_cornel_button(QString icon);
    int get_current_index() const;
    void clear();
    inline tab *get_tab(int index) const;
    inline QList<tab*> get_tabs() const;
    inline int count() const;
    inline bool empty() const;
signals:
    void closed_tab(QString name);
};

inline QList<tab*> tab_widget::get_tabs() const
{
    return tabs;
}

inline bool tab_widget::empty() const
{
    return tabs.empty();
}

inline int tab_widget::count() const
{
    return tabs.count();
}

inline tab *tab_widget::get_tab(int index) const
{
    return tabs.value(index);
}

#endif // TAB_WIDGET_H
