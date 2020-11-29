#include <tab_widget.h>

tab_widget::tab_widget()
    : current_index(0)
{
    this->setObjectName("tab_widget");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    tab_bar = new QWidget(this);
    tab_bar_layout = new QHBoxLayout(tab_bar);
    tab_bar_layout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    tab_bar_layout->setMargin(0);
    tab_bar_layout->setSpacing(5);

    layout->addWidget(tab_bar);

    widget_section = new QWidget(this);
    widget_section_layout = new QVBoxLayout(widget_section);
    widget_section_layout->setMargin(0);

    layout->addWidget(widget_section);

    // boton de la esquina
    cornel_button = new QPushButton(this);
    cornel_button->hide();
    tab_bar_layout->addWidget(cornel_button);
    //
    //
    tab_bar_layout->addStretch();
}

tab_widget::~tab_widget()
{
}

QPushButton *tab_widget::add_cornel_button(QString icon)
{
    qt::set_icon(cornel_button, icon, 25);
    cornel_button->show();

    return cornel_button;
}

void tab_widget::set_index(int index)
{
    current_index = index;

    auto tab = tabs.value(index);
    if (tab)
        set_tab(tab->get_name());
}

void tab_widget::set_tab(QString name)
{
    for (tab *_tab : tabs)
    {
        if (_tab->get_name() == name)
        {
            _tab->setProperty("_checked", true);
            _tab->set_checked(true);
        }
        else
        {
            _tab->setProperty("_checked", false);
            _tab->set_checked(false);
        }
    }
}

void tab_widget::add_tab(QWidget *widget, QString name)
{
    // añade el widget a la seccion de widgets
    widget_section_layout->addWidget(widget);
    //
    //

    tab *_tab = new tab(this, name, widget);
    tabs.push_back(_tab);

    tab_bar_layout->insertWidget(tabs.count(), _tab);

    set_index(current_index);
}