#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>
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
#include <knob_floating.h>

class viewer : public QWidget
{
private:
    QVBoxLayout *layout;

    viewer_gl *_viewer_gl;
    time_line *_time_line;
	QString playing_option;
	QString name;

	int first_frame, last_frame;
	int input, output;
	int current_frame;
	float frame_rate;
	bool in_out;

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
	action *visibility_in_out_action;
	action *input_action;
	action *output_action;
	//

	// General
	void set_frame(int frame);
	void set_frame_rate(float rate);
	void enable_in_out(bool enable);
	void set_in_out(int input, int output);
	//

	// Player
	QLineEdit *frame_edit;
	QLineEdit *skip_frame_edit;
	QLineEdit *input_frame_edit;
	QLineEdit *output_frame_edit;
	combo_box *frame_rate_menu;
	combo_box *play_back_options;
	combo_box *visible_range;
	QTimeLine *qtime_line;
	void player_init();
	void play(QTimeLine::Direction direction);
	void play_finished();
	void set_playing_option(QString option);
	void stop();
	void go_to_first_frame();
	void go_to_last_frame();
	void next_frame();
	void previous_frame();
	void next_key_frame();
	void previous_key_frame();
	void skip_forward();
	void skip_backward();
	pair<int, int> get_current_range() const;
	//
public:
	viewer(QString _name);
    ~viewer();

	void set_frame_range(int _first_frame, int _last_frame);
	QString get_name() const;
};

#endif // VIEWER_H
