#ifndef TIME_LINE_HPP
#define TIME_LINE_HPP

#include <gl_view.h>

class time_line : public gl_view
{
private:
    /* data */
public:
    time_line(/* args */);
    ~time_line();

protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // TIME_LINE_HPP