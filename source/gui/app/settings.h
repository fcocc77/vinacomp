#ifndef SETTINGS_HPP
#define SETTINGS_HPP

// QT5
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

class settings : public QWidget
{
private:
    QVBoxLayout *layout;
    QHBoxLayout *content_layout;

    QWidget *general_widget;
    QWidget *auto_save_widget;
    QWidget *plugins_widget;
    QWidget *appearance_widget;

    void set_content(QString content_name);

    void set_visible_general(bool visible);
    void set_visible_auto_save(bool visible);
    void set_visible_plugins(bool visible);
    void set_visible_appearance(bool visible);

public:
    settings(/* args */);
    ~settings();
};

#endif // SETTINGS_HPP
