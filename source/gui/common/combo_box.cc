#include <combo_box.h>

combo_box::combo_box(QList<combo_box_item> _items, int default_index, QWidget *_parent)
    : items(_items)
    , current_index(0)
    , parent(_parent)
{
    this->setObjectName("combo_box");
    this->setMinimumHeight(20);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setMinimumWidth(70);
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);

    label = new QLabel("-");

    menu = new QMenu(this);
    menu->hide();

    layout->addWidget(label);
    layout->addStretch();

    // Flecha
    QString path = "resources/images/combo_box_arrow_a.png";
    QPixmap pixmap(path);
    arrow = new QLabel(this);
    pixmap = pixmap.scaledToHeight(9, Qt::SmoothTransformation);
    arrow->setPixmap(pixmap);
    //

    layout->addWidget(arrow);

    for (auto item : items)
        add_item(item);

    set_index(default_index, false);
}

combo_box::~combo_box()
{
    for (action *_action : actions)
        delete _action;

    delete label;
    delete menu;
    delete layout;
    delete arrow;
}

void combo_box::add_shortcut(int _index, QString key)
{
    if (_index >= actions.count())
        return;

    action *_action = actions[_index];

    _action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    if (!key.isEmpty())
        _action->setShortcut(QKeySequence(key));

    if (parent)
        parent->addAction(_action);
}

void combo_box::set_index(int _index, bool emit_signal)
{
    if (_index >= actions.count())
        return;

    // cambia el icono a la accion anterior
    actions[current_index]->set_icon("radio_button_unchecked_a");
    //

    current_index = _index;

    action *_action = actions.value(_index);
    _action->set_icon("radio_button_checked_b");
    QString name = _action->get_label();
    label->setText(name);

    if (emit_signal)
        changed(items[current_index].value, current_index); // Signal
}

void combo_box::set_value(QVariant value)
{
    for (int i = 0; i < items.count(); i++)
    {
        if (items[i].value == value)
        {
            set_index(i);
            return;
        }
    }
}

void combo_box::add_item(combo_box_item item)
{
    QString label = item.label;

    action *_action = new action(label, "");

    // por defecto no es un boton, es un checkbox
    if (!item.button)
    {
        _action->set_icon("radio_button_unchecked_a");
        _action->connect_to(this, [=]() { set_value(item.value); });
    }
    else
    {
    }

    actions.push_back(_action);
    menu->addAction(_action);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();
}

