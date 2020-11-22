#include <combo_box.h>

combo_box::combo_box(/* args */)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    label = new QLabel("nombre");

    menu = new QWidget();

    menu_layout = new QVBoxLayout(menu);

    menu->hide();

    layout->addWidget(label);
}

combo_box::~combo_box()
{
}

void combo_box::add_item(QString name)
{
    // this->addItem(item);
    QLabel *item = new QLabel(name);
    menu_layout->addWidget(item);
}

void combo_box::add_items(QStringList list)
{
    // this->addItems(list);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{

    if (!menu->parentWidget())
        menu->setParent(qApp->activeWindow());

    print("ok");
    menu->show();
    this->setFocus();
}

void combo_box::leaveEvent(QEvent *event)
{

    // print("OUT");
    // menu->hide();

    // QFrame::focusOutEvent(event);
}