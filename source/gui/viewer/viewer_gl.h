#ifndef VIEWER_GL_HPP
#define VIEWER_GL_HPP

#include <gl_view.h>
#include <action.h>
#include <util.h>

class viewer_gl : public gl_view
{
private:
    void draw_frame(int width, int height, QColor color);
	void draw_image();
public:
    viewer_gl();
    ~viewer_gl();

    // Acciones
    action *center_viewer;
    action *fit_100_percent;
    //

	QImage *image;
	int image_width, image_height;

	bool fitted;
	void fit_to_viewport();
	void fit_to_percent(int percent);

	void set_image(QImage *image, int image_width = 1920, int image_height = 1080);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif //VIEWER_GL_HPP
