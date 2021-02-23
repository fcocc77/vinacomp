#ifndef VIEWER_GL_HPP
#define VIEWER_GL_HPP

#include <opencv2/opencv.hpp>

#include <gl_view.h>
#include <action.h>
#include <util.h>

class viewer_gl : public gl_view
{
	Q_OBJECT
private:
    // Acciones
    action *center_viewer;
    action *fit_100_percent;
    //

	cv::Mat *image;
	int image_width, image_height;
	bool fitted;

	bool overlay;

    void draw_frame();
	void draw_image();

	void fit_to_viewport();
	void fit_to_percent(int percent);

public:
    viewer_gl();
    ~viewer_gl();

	void set_image(cv::Mat *image);
	void set_overlay(bool _overlay);
	void isolate_channel(int channel);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:
	void right_click();
};

#endif //VIEWER_GL_HPP
