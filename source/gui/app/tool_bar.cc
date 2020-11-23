#include <tool_bar.h>

tool_bar::tool_bar()

{
    this->setMouseTracking(true);
    this->setTabletTracking(true);

    this->setObjectName("tool_bar");
    this->setMaximumWidth(40);

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 10, 2, 10);

    setup_ui();
}

tool_bar::~tool_bar()
{
}

void tool_bar::setup_ui()
{
    int icon_size = 25;

    QPushButton *script_layout = new QPushButton();
    qt::set_icon(script_layout, "view_compact_a", icon_size);
    layout->addWidget(script_layout);

    QPushButton *comp_layout = new QPushButton();
    qt::set_icon(comp_layout, "vertical_split_a", icon_size);
    layout->addWidget(comp_layout);

    layout->addStretch();

    QPushButton *new_project = new QPushButton();
    qt::set_icon(new_project, "add_a", icon_size);
    layout->addWidget(new_project);

    QPushButton *open_project = new QPushButton();
    qt::set_icon(open_project, "folder_a", icon_size);
    layout->addWidget(open_project);

    QPushButton *settings = new QPushButton();
    qt::set_icon(settings, "settings_a", icon_size);
    layout->addWidget(settings);
}
