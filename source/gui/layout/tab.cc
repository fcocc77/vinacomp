#include <tab.h>
#include <tab_widget.h>

tab::tab(QWidget *__tab_widget, QString _name, QWidget *_content)
    : QWidget(__tab_widget),
      _tab_widget(__tab_widget),
      content(_content),
      name(_name),

      _checked(false)
{
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->hide();

    this->setObjectName("tab");

    layout = new QHBoxLayout(this);
    label = new QLabel(name);
    close_button = new QPushButton(this);
    connect(close_button, &QPushButton::clicked, this, [this] {
        dynamic_cast<tab_widget *>(_tab_widget)->close_tab(name);
    });

    qt::set_icon(close_button, "close_a", 10);

    layout->addWidget(label);
    layout->addWidget(close_button);
}

tab::~tab()
{
    delete layout;
    delete close_button;
    delete label;
}

QString tab::get_name() const
{
    return name;
}

QWidget *tab::get_content_widget() const
{
    return content;
}

bool tab::checked() const
{
    return _checked;
}

void tab::set_checked(bool __checked)
{
    _checked = __checked;

    this->style()->unpolish(this);
    this->style()->polish(this);
    label->style()->unpolish(this);
    label->style()->polish(this);

    this->update();
    content->setVisible(_checked);
}

void tab::mousePressEvent(QMouseEvent *event)
{
    dynamic_cast<tab_widget *>(_tab_widget)->set_tab(name);
}