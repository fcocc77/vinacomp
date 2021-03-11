#ifndef GL_VIEW_HPP
#define GL_VIEW_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>

#include <util.h>
#include <qt.h>

class gl_view : public QOpenGLWidget,
                public QOpenGLFunctions
{
	Q_OBJECT

// Manejadores
#include <box_handler.h>
#include <pos_handler.h>
#include <transform_handler.h>
//

private:
    QPoint click_position;
    bool panning, zooming, pressed, lock_scale, center_button;

    QPointF scale, click_scale;
    QPointF translate, click_translate;

public:
    gl_view(bool _lock_scale = false);
    ~gl_view();

    // Draw
    void draw_text(
        QString text,
        QColor color,
        QPointF coords,
        QPointF viewer_position = {-1, -1},
        int font_size = 10,
        Qt::Alignment align = Qt::AlignCenter,
        QPointF offset = {0, 0});
    void draw_point(QPointF coords, QColor color = Qt::white, 
			int size = 7, bool smooth = true);
    void draw_box(QLineF diagonal_line, QColor color, QColor border_color = {0, 0, 0});
    void draw_line(QPointF src, QPointF dst, QColor color, float width = 1);
    void draw_dashed_line(QLineF line, QColor color, int stipple);

    void draw_triangle(QPointF pos, float size, QColor color,
			bool anchor_on_tip = false, float rotate = 0);

	void draw_centered_box(QPointF pos, float size, QColor color, float rotate = 0);

    void draw_circle(
		QPointF anchor_point,
		float ratio, 
		QColor color,
		pair<float, float> scale = {1, 1}, 
		float rotate = 0,
		bool keep_scale = false, 
		int num_segments = 100,
		bool fill = false);
    //
    //

    QList<float> generate_coord_range(
        float separation,
        Qt::Orientation orientation,
        QColor &color,
        QPointF life_range = {0.5, 70},
        bool separation_by_coord = false,
		int _life = 1000);

    void aa_enable(bool enable);
    bool cursor_above_rect(QPoint cursor, QPointF point, QPointF point2) const;
	bool cursor_above_point(QPoint cursor, QPointF, int tolerance = 10) const;
	bool cursor_above_line(QPoint cursor, QLineF line) const;
    QPointF get_coordinate(QPoint cursor_position) const;
    QPointF get_position(QPointF coordinate) const;
    QPointF get_coords(QPoint mouse_position) const;
    QPointF get_coordsf(QPointF mouse_position) const;
    QPointF get_scale() const;
	QPointF get_translate() const;
    void set_scale(QPointF _scale);
    void set_translate(QPointF _translate);
    float get_aspect() const;
    void set_default();
    void set_transform(QPointF _translate, QPointF _scale);
    void set_ortho(float left, float right, float bottom, float top);
    QPointF rotate_point(QPointF point, QPointF anchor_point, float angle, bool keep_aspect = false, bool keep_offset = false) const;
	QPointF arc_point(QPointF anchor_point, int ratio, float angle);
    float get_angle_two_points(QPointF a, QPointF b) const;


protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Eventos
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;
    void showEvent(QShowEvent *event) override;

    virtual void cursor_move_event(QPoint position);
};

#endif //GL_VIEW_HPP
