#include <viewer.hpp>

viewer::viewer(/* args */)
{

    graphics_view *_graphics_view = new graphics_view();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    this->setObjectName("viewer");

    QWidget *info = info_setup_ui();
    QWidget *player = player_setup_ui();
    QWidget *timeline = timeline_setup_ui();
    QWidget *controls = control_setup_ui();
    QWidget *image_correction = image_correction_setup_ui();

    layout->addWidget(controls);
    layout->addWidget(image_correction);
    layout->addWidget(_graphics_view);
    layout->addWidget(info);
    layout->addWidget(timeline);
    layout->addWidget(player);
}

viewer::~viewer()
{
}

QWidget *viewer::image_correction_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("image_correction");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    //

    return widget;
}

QWidget *viewer::control_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("controls");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    //

    QComboBox *channels = new QComboBox();
    layout->addWidget(channels);

    return widget;
}

QWidget *viewer::info_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("info");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    widget->setMinimumHeight(10);
    //

    return widget;
}

QWidget *viewer::timeline_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("timeline");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    widget->setMinimumHeight(50);
    //

    return widget;
}

QWidget *viewer::player_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("player");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    //

    QPushButton *play_forward = new QPushButton();
    QPushButton *play_backward = new QPushButton("play_backward");
    qt::set_icon(play_forward, "play_arrow_a");

    QPushButton *first_frame = new QPushButton();
    QPushButton *last_frame = new QPushButton();
    qt::set_icon(first_frame, "skip_previous_a");
    qt::set_icon(last_frame, "skip_next_a");

    QPushButton *next_frame = new QPushButton("next_frame");
    QPushButton *previous_frame = new QPushButton("previous_frame");

    QPushButton *next_key_frame = new QPushButton("next_key_frame");
    QPushButton *previous_key_frame = new QPushButton("previous_key_frame");

    QPushButton *skip_forward = new QPushButton("skip_forward");
    QPushButton *skip_backward = new QPushButton("skip_backward");

    layout->addWidget(first_frame);
    layout->addWidget(previous_key_frame);
    layout->addWidget(previous_frame);
    layout->addWidget(play_backward);

    layout->addWidget(play_forward);
    layout->addWidget(next_frame);
    layout->addWidget(next_key_frame);
    layout->addWidget(last_frame);

    layout->addWidget(skip_forward);
    layout->addWidget(skip_backward);

    return widget;
}

graphics_view::graphics_view(/* args */)
{

    QGraphicsScene *graphics_scene = new QGraphicsScene();

    QString image_file = "/home/pancho/Pictures/pexels-leonardo-vazquez-3742854.jpg";

    QImage image(image_file);

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    graphics_scene->addItem(item);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    this->setScene(graphics_scene);
}

graphics_view::~graphics_view()
{
}

void graphics_view::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale(1.25, 1.25);
    else
        scale(0.8, 0.8);
}