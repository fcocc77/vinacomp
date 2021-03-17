#include <button.h>

button::button(QWidget *parent, bool _hover)
    : QPushButton(parent)
    , hover(_hover)
{
    if (hover)
    {
        this->setMouseTracking(true);
        this->setTabletTracking(true);
    }
}

button::~button() {}

void button::change_icon(QString name)
{
    QPixmap pixmap("resources/images/" + name + ".png");
    QIcon ButtonIcon(pixmap);
    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(size_icon, size_icon));
}

void button::set_icon(QString name, int size)
{
    size_icon = size;
    normal_icon = name + "_a";
    hover_icon = name + "_c";

    change_icon(normal_icon);
}

void button::set_hover_icon()
{
    change_icon(hover_icon);
}

void button::set_normal_icon()
{
    change_icon(normal_icon);
}