#include <knob_choice.h>
#include <util.h>

knob_choice::knob_choice(knob_props props, QList<combo_box_item> items,
                         int _default_index)
    : knob(props)
    , default_index(_default_index)
{
    this->setObjectName("knob_choice");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(init_space);

    choice = new combo_box(items, default_index);
    choice->setToolTip(get_tips());
    connect(choice, &combo_box::changed, this, [=](QVariant value, int index) {
        changed(value, index);
        to_node_gui(this);
        update_value(QJsonArray{index, value.toString()});
    });

    layout->addWidget(choice);
    layout->addStretch();
}

knob_choice::~knob_choice() {}

void knob_choice::restore_default()
{
    knob::restore_default();
    choice->set_index(default_index, false);
}

void knob_choice::restore_param()
{
    knob::restore_param();

    QJsonArray items = knob_data.value("items").toArray();
    QJsonArray value = get_param_value().toArray();
    int index = value[0].toInt();

    for (QJsonValue item : items)
    {
        QJsonArray _item = item.toArray();
        QString label = _item[0].toString();
        QJsonValue value = _item[1];

        choice->add_item({label, value});
    }

    choice->set_index(index, false);
}

int knob_choice::get_index() const
{
    return choice->get_index();
}

QVariant knob_choice::get_value() const
{
    return choice->get_value();
}
