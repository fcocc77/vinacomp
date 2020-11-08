#ifndef GL_VIEW_HPP
#define GL_VIEW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>

#include <util.hpp>
#include <qt.hpp>

class gl_view : public QOpenGLWidget,
                public QOpenGLFunctions
{
private:
    QPoint click_position;
    bool panning = false;
    bool zooming = false;

    bool zoom_lock;

    QPointF zoom_scale;
    QPointF click_zoom_scale;

    QPointF coord;
    QPointF click_coord;

public:
    gl_view();
    ~gl_view();

    void draw_text(
        QString text,
        QColor color,
        QPointF coords,
        QPointF viewer_position = {-1, -1});

    void draw_line(QPointF src, QPointF dst, QColor color);
    QList<float> generate_coord_range(
        float separation,
        Qt::Orientation orientation,
        QColor &color,
        QPointF life_range = {0.5, 70});

    QPointF get_coordinate(QPoint cursor_position);
    QPointF get_position(QPointF coordinate);
    QPointF map_position(QPoint mouse_position);
    QPointF get_scale();
    void set_default();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    virtual void set_zoom_lock(bool enable);

    // Eventos
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif //GL_VIEW_HPP