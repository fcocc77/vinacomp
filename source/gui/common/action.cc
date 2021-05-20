#include <action.h>
#include <tools.h>
#include <qt.h>
#include <util.h>

action::action(QString _label, QString shortcut_key, QString _icon_name)
    : key(shortcut_key)
    , icon_name(_icon_name)
    , label(_label)
    , checkable(false)
    , checked(false)
    , visible(true)
    , disable(false)
    , illuminated_icon(false)
    , has_shortcut(!shortcut_key.isEmpty())
    , _tools(nullptr)
    , button(nullptr)

{
    this->setText(label);
    set_icon(icon_name);

    this->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    if (has_shortcut)
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

QPushButton *action::make_button(QWidget *__tools, int _icon_size, bool __one_check_at_time)
{
    _tools = __tools;
    _one_check_at_time = __one_check_at_time;
    icon_size = _icon_size;

    if (!button)
    {
        button = new QPushButton();
        if (!object_name.isEmpty())
            button->setObjectName(object_name);
        set_tool_tip(label);
        set_icon(icon_name);

        connect(button, &QPushButton::clicked, this, [=]() {
            if (disable)
                return;

            this->trigger();
        });

        connect(button, &QPushButton::pressed, this, [=]() {
            qt::set_icon(button, icon_name + "_disable", icon_size);
        });

        connect(button, &QPushButton::released, this, [=]() {
            if (disable)
                return;

            set_icon(icon_name);
        });
    }

    return button;
}

void action::set_object_name(QString name)
{
    object_name = name;
}

void action::uncheck_all()
{
    if (!checkable || !_tools || !_one_check_at_time)
        return;

    static_cast<tools *>(_tools)->set_checked_all(false);
}

void action::set_checked(bool _checked, bool emmit_signal)
{
    if (!checkable)
        return;

    if (button && emmit_signal)
    {
        button->click();
        return;
    }

    checked = _checked;

    set_illuminated_button(false);

    this->setChecked(checked);
}

void action::set_illuminated_button(bool illuminated)
{
    illuminated_icon = illuminated;
    set_icon(icon_name);
}

QString action::get_icon_name() const
{
    return icon_name;
}

void action::set_icon(QString _icon_name, QString force_state)
{
    if (_icon_name.isEmpty())
        return;

    icon_name = _icon_name;
    QString icon_state;

    if (force_state.isEmpty())
    {
        if (disable)
            icon_state = icon_name + "_disable";
        else if (illuminated_icon)
            icon_state = icon_name + "_white";
        else if (checked)
        {
            if (!checked_icon_name.isEmpty())
                icon_state = checked_icon_name + "_checked";
            else
                icon_state = icon_name + "_checked";
        }
        else
            icon_state = icon_name + "_normal";
    }
    else
        icon_state = icon_name + "_" + force_state;

    this->setIcon(QIcon("resources/images/" + icon_state + ".png"));

    if (button)
        qt::set_icon(button, icon_state, icon_size);
}

void action::set_checkable(bool _checkable, QString checked_icon)
{
    checked_icon_name = checked_icon;

    checkable = _checkable;
    this->setCheckable(checkable);
}

bool action::is_checked() const
{
    return checked;
}

void action::set_tool_tip(QString tip)
{
    if (has_shortcut)
        tool_tip = tip + ":  " + key;
    else
        tool_tip = tip;

    if (button)
        button->setToolTip(tool_tip);
}

QString action::get_tool_tip() const
{
    return tool_tip;
}

QString action::get_label() const
{
    return label;
}

void action::set_disable(bool _disable)
{
    disable = _disable;
    set_icon(icon_name);
}
