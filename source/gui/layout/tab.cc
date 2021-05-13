#include <tab.h>
#include <tab_widget.h>
#include <knob_editor.h>

tab::tab(QWidget *__tab_widget, QString _name, QWidget *_content,
         bool _has_close_button, QWidget *__knob_editor, QWidget *_panel)

    : QWidget(__tab_widget)
    , _checked(false)
    , _tab_widget(__tab_widget)
    , has_close_button(_has_close_button)
    , visible_close_button(true)
    , name(_name)
    , content(_content)
    , close_button(nullptr)
    , edit_button(nullptr)
    , _knob_editor(__knob_editor)
    , panel(_panel)
{
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->hide();

    this->setObjectName("tab");

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 2, 10, 2);
    label = new QLabel(name);
    layout->addWidget(label);

    if (has_close_button)
        set_visible_close_button(true);
}

tab::~tab()
{
    delete layout;

    if (close_button)
        delete close_button;

    if (edit_button)
        delete edit_button;

    delete label;
}

void tab::set_name(QString _name)
{
    name = _name;
    label->setText(name);
}

void tab::set_visible_close_button(bool visible)
{
    if (!close_button)
    {
        close_button = new button(this, true);
        close_button->set_icon("close", 15);
        close_button->hide();

        connect(close_button, &QPushButton::clicked, this, [this] {
            static_cast<tab_widget *>(_tab_widget)->close_tab(name);
        });

        layout->addWidget(close_button);
    }

    close_button->setVisible(visible);
    set_margin_with_buttons();
}

void tab::set_visible_edit_button(bool visible)
{
    if (!edit_button)
    {
        edit_button = new button(this, true);
        edit_button->set_icon("edit", 15);
        edit_button->hide();

        connect(edit_button, &QPushButton::clicked, this, [this] {
            static_cast<knob_editor *>(_knob_editor)->edit_tab(this);
        });

        layout->addWidget(edit_button);
    }

    edit_button->setVisible(visible);
    set_margin_with_buttons();
}

void tab::set_margin_with_buttons()
{
    bool edit_visible = false;
    if (edit_button)
        edit_visible = edit_button->isVisible();

    bool close_visible = false;
    if (close_button)
        close_visible = close_button->isVisible();

    if (edit_visible || close_visible)
        layout->setContentsMargins(10, 3, 5, 3);
    else
        layout->setContentsMargins(10, 2, 10, 2);
}

void tab::set_checked(bool __checked)
{
    _checked = __checked;

    this->style()->unpolish(this);
    this->style()->polish(this);
    label->style()->unpolish(label);
    label->style()->polish(label);

    this->update();
    content->setVisible(_checked);
}

void tab::mousePressEvent(QMouseEvent *event)
{
    static_cast<tab_widget *>(_tab_widget)->set_tab(name);
}
