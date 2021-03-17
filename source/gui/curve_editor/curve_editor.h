#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QWidget>

#include <curve_view.h>
#include <tools.h>

class curve_editor : public QWidget
{
private:
    curve_view *view;
    QTreeWidget *tree;

    void setup_ui();
    QTreeWidget *knobs_tree_setup_ui();
    void add_item(QString node, QString param, QString dimension);

public:
    curve_editor(/* args */);
    ~curve_editor();

    void add_curve();
};

#endif // CURVE_EDITOR_HPP