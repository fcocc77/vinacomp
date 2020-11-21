#ifndef VIEWER_GL_HPP
#define VIEWER_GL_HPP

#include <gl_view.hpp>

class viewer_gl : public gl_view
{
private:
    void draw_frame(int width, int height, QColor color);

public:
    viewer_gl();
    ~viewer_gl();

protected:
    void initializeGL() override;
    void paintGL() override;

    // void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif //VIEWER_GL_HPP