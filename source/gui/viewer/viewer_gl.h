#ifndef VIEWER_GL_HPP
#define VIEWER_GL_HPP

#include <opencv2/opencv.hpp>

#include <gl_view.h>
#include <action.h>
#include <util.h>
#include <render_data.h>
#include <knob.h>

class viewer_gl : public gl_view
{
	Q_OBJECT
private:
    // Acciones
    action *center_viewer;
    action *fit_100_percent;
    //

	QList <QWidget*> panels;
	QWidget *_properties;

	render_data *rdata;
	bool fitted;

	bool overlay;

    void draw_frame();
	void draw_bbox();
	void draw_image();
	void draw_handlers();

	knob *get_knob(QString panel_name, QString knob_name);
	void fit_to_viewport();
	void fit_to_percent(int percent);

public:
    viewer_gl(render_data *rdata, QWidget *_vinacomp);
    ~viewer_gl();

	void handlers_update();
	void set_overlay(bool _overlay);
	void isolate_channel(int channel);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

	void box_handler_changed(QRect box, QString name) override;
	void box_handler_finished(QRect box, QString name) override;
signals:
	void right_click();
};

#endif //VIEWER_GL_HPP
