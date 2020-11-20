#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QLabel>

#include <curve_view.hpp>
#include <tools.hpp>

class curve_editor : public QWidget
{
private:
    curve_view *view;

    void setup_ui();
    QTreeWidget *knobs_tree_setup_ui();

public:
    curve_editor(/* args */);
    ~curve_editor();
};

#endif // CURVE_EDITOR_HPP