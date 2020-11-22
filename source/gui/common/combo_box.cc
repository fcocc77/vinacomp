#include <combo_box.h>

combo_box::combo_box()
    : current_index(0)
{
    this->setObjectName("combo_box");
    this->setMinimumWidth(70);
    this->setMaximumWidth(100);
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

void combo_box::set_index(int _index)
{
    current_index = _index;

    action *_action = actions.value(_index);
    label->setText(_action->get_label());
}

void combo_box::set_value(QString name)
{
    for (int i = 0; i < actions.count(); i++)
    {
        action *_action = actions[i];
        if (_action->get_label() == name)
        {
            set_index(i);
            return;
        }
    }
}

void combo_box::add_item(QString name)
{
    action *_action = new action(name, "K", "close_a");
    _action->connect_to(this, [=]() {
        set_value(name);
    });

    actions.push_back(_action);
    menu->addAction(_action);

    set_index(current_index);
}

void combo_box::add_items(QStringList list)
{
    for (QString name : list)
        add_item(name);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();
}
