#include <tool_bar.h>

tool_bar::tool_bar(maker *__maker)
    : _maker(__maker)
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

void tool_bar::add_menu(QString group, QString icon_group)
{
    int icon_size = 25;

    QMenu *menu = new QMenu(this);
    for (QJsonValue value : _maker->get_effects(group))
    {
        QJsonObject effect = value.toObject();
        QString label = effect["label"].toString();
        QString id = effect["id"].toString();
        QString icon = effect["icon"].toString();

        QAction *effect_action = new QAction(label);
        effect_action->setIcon(QIcon("resources/images/" + icon + ".png"));
        menu->addAction(effect_action);

        connect(effect_action, &QAction::triggered, this, [=]() {
            _maker->create_fx(id);
        });
    }
    QPushButton *popup_button = new QPushButton();
    connect(popup_button, &QPushButton::clicked, this, [=]() {
        menu->popup(QCursor::pos());
        menu->show();
    });
    qt::set_icon(popup_button, icon_group, icon_size);
    layout->addWidget(popup_button);
}

void tool_bar::setup_ui()
{
    int icon_size = 25;

    add_menu("image", "image_a");
    add_menu("draw", "brush_a");
    add_menu("time", "time_a");
    add_menu("channel", "channel_a");
    add_menu("color", "color_a");
    add_menu("filter", "filter_a");
    add_menu("keyer", "keyer_a");
    add_menu("merge", "merge_a");
    add_menu("transform", "transform_a");
    add_menu("other", "other_a");

    layout->addStretch();

    QPushButton *script_layout = new QPushButton();
    qt::set_icon(script_layout, "view_compact_a", icon_size);
    layout->addWidget(script_layout);

    QPushButton *comp_layout = new QPushButton();
    qt::set_icon(comp_layout, "vertical_split_a", icon_size);
    layout->addWidget(comp_layout);
}
