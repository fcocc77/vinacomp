#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>

#include <graphics_view.hpp>


class curve_editor : public QWidget
{
private:
    void setup_ui();
    QTreeWidget *knobs_tree_setup_ui();

public:
    curve_editor(/* args */);
    ~curve_editor();
};

#endif // CURVE_EDITOR_HPP