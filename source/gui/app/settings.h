#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include <QTreeWidget>
#include <QSplitter>

class settings : public QWidget
{
private:
    struct item_struct
    {
        QString name;
        QWidget *widget;
        QTreeWidgetItem *item;
    };

    QList<item_struct> items;
    QVBoxLayout *layout;
    QHBoxLayout *content_layout;
    QTreeWidget *tree;

public:
    settings();
    ~settings();

    QVBoxLayout *add_item(QString name);
    void set_content(QString content_name);
};

#endif // SETTINGS_H
