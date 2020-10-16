#include <viewer.hpp>

viewer::viewer(/* args */)
{

    QGraphicsView *graphics_view = new QGraphicsView();
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(graphics_view);
    this->setLayout(layout);
}

viewer::~viewer()
{
}