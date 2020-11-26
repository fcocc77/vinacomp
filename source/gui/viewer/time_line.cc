#include <time_line.h>

time_line::time_line()
    : frame(10)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(50);
}

time_line::~time_line()
{
}

void time_line::mouseMoveEvent(QMouseEvent *event)
{
    if (!qt::alt())
    {
        frame = round(get_coords(event->pos()).x());
        update();
    }

    gl_view::mouseMoveEvent(event);
}