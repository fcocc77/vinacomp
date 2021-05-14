#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>

#include <tools.h>

class file_dialog : public QWidget
{
private:
    QVBoxLayout *layout;

    tools *tool_bar;
    QWidget *center_widget, *tag_widget, *directory_widget, *preview_widget;
    QWidget *bottom_widget;
    QTreeWidget *tree;

public:
    file_dialog(QWidget *parent);
    ~file_dialog();
};

#endif // FILE_DIALOG_H
