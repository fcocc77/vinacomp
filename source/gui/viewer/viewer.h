#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTimeLine>

#include <qt.h>
#include <util.h>
#include <combo_box.h>
#include <slider.h>
#include <viewer_gl.h>
#include <time_line.h>
#include <tools.h>
#include <action.h>

class viewer : public QWidget
{
private:
    QVBoxLayout *layout;

    viewer_gl *_viewer_gl;
    time_line *_time_line;

	int current_frame;
	int frame_rate;
    //
    void setup_ui();
    QWidget *player_setup_ui();
    QWidget *info_setup_ui();
    QWidget *control_setup_ui();
    QWidget *image_correction_setup_ui();
    //
    //
	
	// Acciones
	action *play_forward_action;
	action *stop_forward_action;
	action *play_backward_action;
	action *stop_backward_action;
	//

	// Player
	QLineEdit *frame_edit;
	QLineEdit *skip_frame_edit;
	QLineEdit *input_frame_edit;
	QLineEdit *output_frame_edit;
	QTimeLine *qtime_line;
	void player_init();
	void set_frame(int frame);
	void play(QTimeLine::Direction direction);
	void stop();
	void go_to_first_frame();
	void go_to_last_frame();
	void next_frame();
	void previous_frame();
	void next_key_frame();
	void previous_key_frame();
	void skip_forward();
	void skip_backward();
	//
public:
    viewer(/* args */);
    ~viewer();
};

#endif // VIEWER_H
