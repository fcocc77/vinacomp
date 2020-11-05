#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <graphics_view.hpp>

class curve_view : public QOpenGLWidget, public QOpenGLFunctions
{

private:
    QPointF point;

    QPointF map_position(QPoint position);

public:
    curve_view(/* args */);
    ~curve_view();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Eventos
    void mousePressEvent(QMouseEvent *event);
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