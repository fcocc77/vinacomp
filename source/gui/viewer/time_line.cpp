#include <time_line.hpp>

time_line::time_line(/* args */)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(50);
}

time_line::~time_line()
{
}

void time_line::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0.2, 1);
}

void time_line::paintGL()
{
    gl_view::paintGL();
}