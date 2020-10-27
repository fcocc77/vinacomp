#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QPainter>

#include <graphics_view.hpp>

class curve_view : public graphics_view
{
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

public:
    curve_view(/* args */);
    ~curve_view();
};

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