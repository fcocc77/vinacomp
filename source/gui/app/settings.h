#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include <QTreeWidget>
#include <QSplitter>
#include <QPushButton>

#include <knob.h>

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

protected:
    QWidget *_vinacomp;
    QMap<QString, knob *> *knobs;

    virtual void save_settings();
    knob *get_knob(QString name) const;

public:
    settings(bool has_dialog_buttons = false, QWidget *_vinacomp = 0);
    ~settings();

    QVBoxLayout *add_item(QString name);
    void set_content(QString content_name);
};

#endif // SETTINGS_H
