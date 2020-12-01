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
    bool dragging;

    int first_frame, last_frame;

    // Paint
    void draw_coordinate_numbers();
    void draw_cursor();
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