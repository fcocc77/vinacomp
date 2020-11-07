#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QPainter>

#include <gl_view.hpp>

class curve_view : public gl_view
{

private:
    QPointF point;

    void draw_grid();
    void draw_line(QPointF src, QPointF dst, QColor color);

public:
    curve_view(/* args */);
    ~curve_view();

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event);
};

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