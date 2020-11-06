#ifndef GL_VIEW_HPP
#define GL_VIEW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

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

    QPointF get_coordinate(QPoint cursor_position);
    QPointF map_position(QPoint mouse_position);

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