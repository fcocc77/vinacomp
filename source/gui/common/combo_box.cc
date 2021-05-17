#include <combo_box.h>
#include <util.h>

combo_box::combo_box(QList<combo_box_item> input_items, int default_index, QWidget *_parent)
    : parent(_parent)
    , current_index(0)
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
    QString path = "resources/images/combo_box_arrow_normal.png";
    QPixmap pixmap(path);
    arrow = new QLabel(this);
    pixmap = pixmap.scaledToHeight(9, Qt::SmoothTransformation);
    arrow->setPixmap(pixmap);
    //

    layout->addWidget(arrow);

    for (auto item : input_items)
        add_item(item);

    set_index(default_index, false);
}

combo_box::~combo_box()
{
    clear();

    delete label;
    delete menu;
    delete layout;
    delete arrow;
}

void combo_box::clear()
{
    for (action *_action : actions)
        delete _action;

    actions.clear();
    items.clear();
    menu->clear();
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
    combo_box_item last_item = items[current_index];
    if (last_item.button)
        actions[current_index]->set_icon(last_item.icon_name, "normal");
    else
        actions[current_index]->set_icon("radio_button_unchecked", "normal");
    //

    current_index = _index;

    action *_action = actions.value(_index);

    auto _item = items.value(_index);
    if (_item.button)
        _action->set_icon(_item.icon_name, "hi");
    else
        _action->set_icon("radio_button_checked", "hi");

    QString name = _action->get_label();
    label->setText(name);

    if (emit_signal)
        changed(items[current_index].value, current_index); // Signal
}

void combo_box::set_value(QVariant value, bool emit_signal)
{
    for (int i = 0; i < items.count(); i++)
    {
        if (items[i].value == value)
        {
            set_index(i, emit_signal);
            return;
        }
    }
}

int combo_box::add_item(combo_box_item item)
{
    QString label = item.label;

    action *_action = new action(label, "");

    // por defecto no es un boton, es un checkbox
    if (!item.button)
    {
        _action->set_icon("radio_button_unchecked", "normal");
        _action->connect_to(this, [=]() { set_value(item.value); });
    }
    else
    {
        if (!item.icon_name.isEmpty())
            _action->set_icon(item.icon_name, "normal");
    }

    int index = items.count();

    items.push_back(item);
    actions.push_back(_action);
    menu->addAction(_action);

    return index;
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    pre_open();
    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();
}

