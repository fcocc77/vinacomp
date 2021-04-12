#include <knob_choice.h>

knob_choice::knob_choice(QList<combo_box_item> items, int default_index)
{
    this->setObjectName("knob_choice");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(init_space);

    choice = new combo_box(items, default_index);
    connect(choice, &combo_box::changed, this, &knob_choice::changed);

    layout->addWidget(choice);
    layout->addStretch();
}

knob_choice::~knob_choice() {}

void knob_choice::restore_param()
{
    knob::restore_param();

    // QJsonArray items = knob_object.value("items").toArray();
    // QJsonArray default_value = knob_object.value("default").toArray();
    // int default_index = default_value[0].toInt();
    // int index;

    // if (data->contains(name))
        // index = data->value(name).toInt();
    // else
        // index = default_index;

    // // convierte la lista de items
    // QList<combo_box_item> _items;
    // for (QJsonValue item : items)
    // {
        // QJsonArray _item = item.toArray();
        // QString label = _item[0].toString();
        // QJsonValue value = _item[1];
        // _items.push_back({label, value});
    // }
    // //
}

int knob_choice::get_index() const
{
    return choice->get_index();
}

QVariant knob_choice::get_value() const
{
    return choice->get_value();
}
