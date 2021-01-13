#ifndef TIME_LINE_HPP
#define TIME_LINE_HPP

#include <QLineEdit>


#include <gl_view.h>
#include <qt.h>
#include <util.h>
#include <action.h>

class time_line : public gl_view
{
	Q_OBJECT
private:
    int frame;
    int ghost_frame;
    bool dragging, ghost_frame_visible, selector_visible;

    int click_x_coords;
	bool is_fit_to_selector;
	pair<int, int> selector;

    int first_frame, last_frame;
    int input, output, click_input, click_output;
    pair<bool, bool> click_over_in_out;

    bool right_button, middle_button, left_button;
    bool dragging_input, dragging_output;
	bool in_out_visible;

	float top_y, mid_y1, mid_y2, number_y;

	QJsonObject palette;

    void fit_to_range();
	void fit_switch();
	void fit_to_selector();
    void set_in_out(int _input, int _output);

	// Input Output
    void drag_in_out(int _frame);
    void set_frame(int _frame);
    pair<bool, bool> over_in_out(int x) const;
    void change_in_out_with_control();
	//

	// Paint
	void draw_selector();
    void draw_coordinate_numbers();
    void draw_cursor();
    void draw_in_out();
	void draw_guide_frames();
    //
    //

	// From viewer
	QLineEdit *frame_edit;
	QLineEdit *input_frame_edit;
	QLineEdit *output_frame_edit;
	QLineEdit *skip_frame_edit;
	//
	//

public:
    time_line(
		QWidget *parent,
		QLineEdit *frame_edit,
		QLineEdit *input_frame_edit,
		QLineEdit *output_frame_edit,
		QLineEdit *skip_frame_edit
	);
    ~time_line();

	void go_to_frame(int _frame);
	void next_frame();
	void previous_frame();
	void next_frame_each(int frames);
	void previous_frame_each(int frames);
	void set_frame_range(int _first_frame, int _last_frame);
    void update_in_out(int _input, int _output);
	void set_in_out_visible(bool visible);

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void cursor_move_event(QPoint position) override;
	void focusOutEvent(QFocusEvent *event) override;

signals:
	void frame_changed(int frame);
	void in_out_changed(int input, int output);
};

#endif // TIME_LINE_HPP
