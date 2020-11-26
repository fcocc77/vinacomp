#ifndef TIME_LINE_HPP
#define TIME_LINE_HPP

#include <gl_view.h>
#include <qt.h>

class time_line : public gl_view
{
private:
    int frame;

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

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // TIME_LINE_HPP