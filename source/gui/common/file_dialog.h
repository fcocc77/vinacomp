#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>

#include <tools.h>

class file_dialog : public QDialog
{
private:
    QVBoxLayout *layout;

    tools *tool_bar;
    QWidget *center_widget, *bookmark_widget, *directory_widget, *preview_widget;
    QWidget *bottom_widget;
    QTreeWidget *tree, *bookmark_tree;
    QList<QTreeWidgetItem*> items;

    QLineEdit *path_edit;

    struct bookmark
    {
        QString name;
        QString path;
        QTreeWidgetItem *item;
    };

    QMap<QString, bookmark> bookmarks;

    QString current_dir;
    void update();
    void go_to_parent();
    void enter_to_dir(QString dirname);
    void add_bookmark();
    void remove_bookmark();
    void open_bookmark(QString bookmark);

public:
    file_dialog(QWidget *parent);
    ~file_dialog();
};

#endif // FILE_DIALOG_H
