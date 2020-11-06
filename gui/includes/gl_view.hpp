#ifndef GL_VIEW_HPP
#define GL_VIEW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <util.hpp>

class gl_view : public QOpenGLWidget, public QOpenGLFunctions
{
private:
    float zoomScale;

    int x1_range = 1000;
    int x2_range = 1000;
    float cursor_x;
    float cursor_y;

    void zoom();

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
};

#endif //GL_VIEW_HPP