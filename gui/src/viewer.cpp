#include <viewer.hpp>

viewer::viewer(/* args */)
{

    QGraphicsView *graphics_view = new QGraphicsView();
    QGraphicsScene *graphics_scene = new QGraphicsScene();

    QHBoxLayout *layout = new QHBoxLayout();

    QString image_file = "/home/pancho/Pictures/pexels-leonardo-vazquez-3742854.jpg";

    QImage image(image_file);

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    graphics_scene->addItem(item);

    graphics_view->setScene(graphics_scene);

    layout->addWidget(graphics_view);
    this->setLayout(layout);
}

viewer::~viewer()
{
}