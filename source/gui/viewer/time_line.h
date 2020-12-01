#ifndef TIME_LINE_HPP
#define TIME_LINE_HPP

#include <gl_view.h>
#include <qt.h>
#include <action.h>

class time_line : public gl_view
{
private:
    int frame;
    int ghost_frame;
    bool dragging, ghost_frame_visible;

    int click_x_coords;

    int first_frame, last_frame;
    int input, output;

    bool right_button, middle_button;

    bool dragging_input, dragging_output;

    void drag_in_out(int _frame);
    void change_frame(int _frame);
    pair<bool, bool> over_in_out(int x) const;
    void fit_to_range();
    void change_in_out_with_control();

    // Paint
    void draw_coordinate_numbers();
    void draw_cursor();
    void draw_in_out();
    //
    //

public:
    time_line(/* args */);
    ~time_line();

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void cursor_move_event(QPoint position) override;
};

#endif // TIME_LINE_HPP