#include <combo_box.h>

combo_box::combo_box(QStringList items, int default_index)
{
	this->setObjectName("combo_box");
	this->setMinimumHeight(20);
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

	for (QString item : items)
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

void combo_box::set_index(int _index, bool emit_signal)
{
	if (_index >= actions.count())
		return;

    current_index = _index;

    action *_action = actions.value(_index);
	_action->set_icon("radio_button_checked_b");
	QString name = _action->get_label();
    label->setText(name);

	if (emit_signal)
		changed(name, current_index); // Signal
}

void combo_box::set_value(QString name)
{
    for (int i = 0; i < actions.count(); i++)
    {
        action *_action = actions[i];
		_action->set_icon("radio_button_unchecked_a");

		if (_action->get_label() == name)
			set_index(i);
    }
}

void combo_box::add_item(QString name)
{
    action *_action = new action(name, "");
	_action->set_icon("radio_button_unchecked_a");
    _action->connect_to(this, [=]() {
        set_value(name);
    });

    actions.push_back(_action);
    menu->addAction(_action);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();
}
