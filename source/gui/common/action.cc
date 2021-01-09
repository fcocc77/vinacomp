#include <action.h>

action::action(QString _label, QString shortcut_key, QString _icon_name)
    : key(shortcut_key),
      icon_name(_icon_name),
      label(_label),
	  checkable(false),
	  checked(false)

{
    this->setText(label);
    if (!icon_name.isEmpty())
        this->setIcon(QIcon("resources/images/" + icon_name + "_a.png"));

    this->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	if (!key.isEmpty())
		this->setShortcut(QKeySequence(key));
}
action::~action()
{
}

QString action::get_icon_name() const
{
    return icon_name;
}

void action::set_checkable(bool _checkable)
{
	checkable = _checkable;
}

bool action::is_checked() const
{
	return checked;
}

void action::set_tool_tip(QString tip)
{
    tool_tip = tip;
}

QString action::get_tool_tip() const
{
    return tool_tip;
}

QString action::get_label() const
{
    return label;
}
