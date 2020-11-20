#include <action.hpp>

action::action(QString _label, QString shortcut_key, QString _icon_name)
    : key(shortcut_key),
      icon_name(_icon_name),
      label(_label)
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

void action::set_tool_tip(QString tip)
{
    tool_tip = tip;
}

QString action::get_tool_tip()
{
    return tool_tip;
}

QString action::get_label()
{
    return label;
}