#include <tool_bar.hpp>

tool_bar::tool_bar(/* args */)
{
    this->setObjectName("tool_bar");
    this->setMaximumWidth(40);

    layout = new QVBoxLayout();
    layout->setContentsMargins(2, 10, 2, 10);
    this->setLayout(layout);

    setup_ui();
}

tool_bar::~tool_bar()
{
}

void tool_bar::setup_ui()
{

    int icon_size = 25;

    QPushButton *image = new QPushButton();
    qt::set_icon(image, "image_a", icon_size);
    layout->addWidget(image);

    QPushButton *draw = new QPushButton();
    qt::set_icon(draw, "brush_a", icon_size);
    layout->addWidget(draw);

    QPushButton *time = new QPushButton();
    qt::set_icon(time, "time_a", icon_size);
    layout->addWidget(time);

    QPushButton *channel = new QPushButton();
    qt::set_icon(channel, "channel_a", icon_size);
    layout->addWidget(channel);

    QPushButton *color = new QPushButton();
    qt::set_icon(color, "color_a", icon_size);
    layout->addWidget(color);

    QPushButton *filter = new QPushButton();
    qt::set_icon(filter, "filter_a", icon_size);
    layout->addWidget(filter);

    QPushButton *keyer = new QPushButton();
    qt::set_icon(keyer, "keyer_a", icon_size);
    layout->addWidget(keyer);

    QPushButton *merge = new QPushButton();
    qt::set_icon(merge, "merge_a", icon_size);
    layout->addWidget(merge);

    QPushButton *transform = new QPushButton();
    qt::set_icon(transform, "transform_a", icon_size);
    layout->addWidget(transform);

    QPushButton *other = new QPushButton();
    qt::set_icon(other, "other_a", icon_size);
    layout->addWidget(other);

    layout->addStretch();

    QPushButton *script_layout = new QPushButton();
    qt::set_icon(script_layout, "view_compact_a", icon_size);
    layout->addWidget(script_layout);

    QPushButton *comp_layout = new QPushButton();
    qt::set_icon(comp_layout, "vertical_split_a", icon_size);
    layout->addWidget(comp_layout);
}