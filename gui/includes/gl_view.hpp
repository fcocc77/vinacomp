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

    bool lock_scale;

    QPointF scale;
    QPointF click_scale;

    QPointF translate;
    QPointF click_translate;

public:
    gl_view(bool _lock_scale = false);
    ~gl_view();

    // Draw
    void draw_text(
        QString text,
        QColor color,
        QPointF coords,
        QPointF viewer_position = {-1, -1});
    void draw_point(QPointF coords, QColor color = Qt::white, int size = 7, bool smooth = true);
    void draw_box(QLineF diagonal_line, QColor color, QColor border_color);
    void draw_line(QPointF src, QPointF dst, QColor color);
    void draw_dashed_line(QLineF line, QColor color, int stipple);
    //
    //

    QList<float> generate_coord_range(
        float separation,
        Qt::Orientation orientation,
        QColor &color,
        QPointF life_range = {0.5, 70},
        bool separation_by_coord = false);

    void aa_enable(bool enable);
    bool is_cursor_above(QPoint cursor, QPointF point, QPointF point2 = {});
    QPointF get_coordinate(QPoint cursor_position);
    QPointF get_position(QPointF coordinate);
    QPointF get_coords(QPoint mouse_position);
    QPointF get_coordsf(QPointF mouse_position);
    QPointF get_scale();
    float get_aspect();
    void set_default();
    void set_transform(QPointF _translate, QPointF _scale);
    void set_ortho(float left, float right, float bottom, float top);
    QPointF rotate_point(QPointF point, QPointF anchor_point, float angle, bool keep_aspect = false);
    float get_angle_two_points(QPointF a, QPointF b);

protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Eventos
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif //GL_VIEW_HPP