#include <button.h>
#include <qt.h>

button::button(QWidget *parent, bool _hover)
    : QPushButton(parent)
    , hover(_hover)
    , checkable(false)
    , checked(false)
    , disable(false)
{
    if (hover)
    {
        this->setMouseTracking(true);
        this->setTabletTracking(true);
    }

    connect(this, &QPushButton::clicked, this, [=]() {
        if (checkable)
        {
            checked = !checked;
            set_checked(checked);
        }

        clicked(checked);
    });
}

button::~button() {}

void button::change_icon(QString name)
{
    QPixmap pixmap("resources/images/" + name + ".png");
    if (name.contains("/"))
        pixmap = QPixmap(name);
    QIcon ButtonIcon(pixmap);
    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(size_icon, size_icon));
}

void button::set_icon(QString name, int size)
{
    size_icon = size;
    if (name.contains("/"))
    {
        normal_icon = name;
        hover_icon = name;
    }
    else
    {
        normal_icon = name + "_normal";
        hover_icon = name + "_checked";
    }

    change_icon(normal_icon);
}

void button::set_checked(bool _checked)
{
    checked = _checked;

    qt::set_property(this, "active", checked);
    this->setChecked(checked);

    if (checked)
        set_hover_icon();
    else
        set_normal_icon();
}

void button::set_disable(bool _disable)
{
    disable = _disable;
    qt::set_property(this, "disable", disable);
    this->setDisabled(_disable);
}

void button::set_hover_icon()
{
    change_icon(hover_icon);
}

void button::set_normal_icon()
{
    change_icon(normal_icon);
}
