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

    float zoom_scale;
    float click_zoom_scale;

    int x1_range = 1000;
    int x2_range = 1000;
    float cursor_x = 0;
    float cursor_y = 0;

    QPointF coord;
    QPointF click_coord;

    void zoom();
    QPointF get_coordinate(QPoint cursor_position);

public:
    gl_view(/* args */);
    ~gl_view();

    QPointF map_position(QPoint mouse_position);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Eventos
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif //GL_VIEW_HPP