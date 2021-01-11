#include <action.h>
#include <tools.h>

action::action(QString _label, QString shortcut_key, QString _icon_name)
    : key(shortcut_key)
	, icon_name(_icon_name)
	, label(_label)
	, checkable(false)
	, checked(false)
	, button(nullptr)
	, visible(true)

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
	if (button)
		delete button;
}

void action::set_visible(bool _visible)
{
	visible = _visible;
	if (button)
		button->setVisible(visible);
}

void action::update()
{
	// actualiza la actual visibilidad
	set_visible(visible);
}

QPushButton *action::make_button(QWidget *_tools, int _icon_size, bool uncheck_all)
{
	icon_size = _icon_size;

	if (!button)
	{
		button = new QPushButton();
		button->setToolTip(label);
		qt::set_icon(button, icon_name + "_a", icon_size);

		connect(button, &QPushButton::clicked, this, [=]() {
			if (checkable)
			{
				bool _checked = checked;
				if (uncheck_all)
					dynamic_cast< tools * >(_tools)->set_checked_all(false);
				set_checked(!_checked);
			}
			this->trigger();
		});
	}

	return button;
}

void action::set_checked(bool _checked)
{
	checked = _checked;

	if (checked)
		qt::set_icon(button, icon_name + "_c", icon_size);
	else
		qt::set_icon(button, icon_name + "_a", icon_size);
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
