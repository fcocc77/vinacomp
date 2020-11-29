#include <tab_widget.h>

tab_widget::tab_widget(bool _has_close_button)
    : current_index(0),
      has_close_button(_has_close_button)
{
    this->setObjectName("tab_widget");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    tab_bar = new QWidget(this);
    tab_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tab_bar_layout = new QHBoxLayout(tab_bar);
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
    delete cornel_button;
    delete tab_bar_layout;
    delete layout;
    delete tab_bar;
    delete widget_section;
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

int tab_widget::get_index_by_name(QString name) const
{
    for (int i = 0; i < tabs.count(); i++)
        if (tabs[i]->get_name() == name)
            return i;

    return -1;
}

void tab_widget::set_tab(QString name)
{
    for (int i = 0; i < tabs.count(); i++)
    {
        tab *_tab = tabs[i];
        if (_tab->get_name() == name)
        {
            _tab->setProperty("_checked", true);
            _tab->set_checked(true);
            current_index = i;
        }
        else
        {
            _tab->setProperty("_checked", false);
            _tab->set_checked(false);
        }
    }
}

void tab_widget::delete_tab(tab *_tab)
{
    QWidget *widget = _tab->get_content_widget();
    widget->hide();
    widget->setParent(0);
    _tab->setParent(0);
    delete _tab;
}

void tab_widget::close_tab(QString name)
{
    int index = get_index_by_name(name);
    if (index == -1)
        return;

    tab *_tab = tabs[index];
    delete_tab(_tab);

    tabs.removeAt(index);

    set_index(0);
}

void tab_widget::clear()
{
    for (tab *_tab : tabs)
        delete_tab(_tab);

    tabs.clear();
}

void tab_widget::add_tab(QWidget *widget, QString name)
{
    // si no es -1 el tab ya existe
    int index = get_index_by_name(name);
    if (index != -1)
    {
        set_index(index);
        return;
    }
    //
    //

    // añade el widget a la seccion de widgets, pero si el widget, ya esta
    // dentro de otro 'tab_widget', busca el otro 'tab_widget' y cierra el tab antes de
    // añadirlo a este.
    auto *parent = widget->parent();
    if (parent)
    {
        tab_widget *old_tab_widget = dynamic_cast<tab_widget *>(parent->parent());
        if (old_tab_widget)
            old_tab_widget->close_tab(name);
    }
    widget_section_layout->addWidget(widget);
    //
    //

    tab *_tab = new tab(this, name, widget, has_close_button);
    tabs.push_back(_tab);

    tab_bar_layout->insertWidget(tabs.count(), _tab);

    set_index(tabs.count() - 1);
}

int tab_widget::get_current_index() const
{
    return current_index;
}
