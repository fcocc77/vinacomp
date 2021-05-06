#include <tab.h>
#include <tab_widget.h>

tab::tab(QWidget *__tab_widget, QString _name, QWidget *_content, bool _has_close_button)

    : QWidget(__tab_widget)
    ,_checked(false)
    , _tab_widget(__tab_widget)
    , has_close_button(_has_close_button)
    , visible_close_button(true)
    , name(_name)
    , content(_content)
    , close_button(nullptr)
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
    delete label;
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
    if (visible)
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
