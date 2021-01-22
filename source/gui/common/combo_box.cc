#include <combo_box.h>

combo_box::combo_box(QList <pair<QString, QVariant>> _items, int default_index)
	: items(_items)
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
		changed(items[current_index].second, current_index); // Signal
}

void combo_box::set_value(QVariant value)
{
    for (int i = 0; i < items.count(); i++)
    {
		if (items[i].second == value)
		{
			set_index(i);
			return;
		}
    }
}

void combo_box::add_item(pair <QString, QVariant> item)
{
	QString label = item.first;

    action *_action = new action(label, "");
	_action->set_icon("radio_button_unchecked_a");
    _action->connect_to(this, [=]() {
        set_value(item.second);
    });

    actions.push_back(_action);
    menu->addAction(_action);
}

void combo_box::mousePressEvent(QMouseEvent *event)
{
    menu->popup(this->mapToGlobal({0, this->height()}));
    menu->show();
}

int combo_box::get_index() const
{
	return current_index;
}

QVariant combo_box::get_value() const
{
	return items.value(current_index).second;
}
