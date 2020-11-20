#include <action.hpp>

action::action(QString label, QString shortcut_key, QString _icon_name)
    : key(shortcut_key),
      icon_name(_icon_name)
{
    this->setText(label);

    this->setIcon(QIcon("resources/images/" + icon_name + ".png"));
    this->setShortcut(QString(key));
}
action::~action()
{
    delete _shortcut;
}

QString action::get_icon_name()
{
    return icon_name;
}