#include <knob_channels.h>

knob_channels::knob_channels(global *_glob)
    : glob(_glob)
{
    this->setObjectName("new_layer");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    layout = new QHBoxLayout(this);
    layout->setMargin(0);

    edit = new QLineEdit;
    // edit->hide();

    accept_button = new button();
    // accept_button->hide();
    connect(accept_button, &QPushButton::clicked, this,
            &knob_channels::accept);

    cancel_button = new button();
    // cancel_button->hide();
    connect(cancel_button, &QPushButton::clicked, this,
            &knob_channels::cancel);

    accept_button->setText("OK");
    cancel_button->setText("Cancel");

    red = new knob_check_box("", true);
    red->setObjectName("red");

    green = new knob_check_box("", true);
    green->setObjectName("green");

    blue = new knob_check_box("", true);
    blue->setObjectName("blue");

    alpha = new knob_check_box("", true);
    alpha->setObjectName("alpha");

    // Layers
    combo_box *layers =
        new combo_box({{"main : rgba", "rgba"}, {"New", "new", true}});

    connect(layers, &combo_box::changed, this,
            [=](QVariant value, int index) { change_layer(value.toString()); });

    layers->get_action(1)->connect_to(this, [=]() { visible_layer_edit(true); });
    //

    layout->addWidget(init_space);

    layout->addWidget(layers);

    layout->addWidget(red);
    layout->addWidget(green);
    layout->addWidget(blue);
    layout->addWidget(alpha);

    layout->addWidget(edit);

    // layout->addStretch();

    layout->addWidget(accept_button);
    layout->addWidget(cancel_button);

}

knob_channels::~knob_channels() {}

void knob_channels::change_layer(QString layer) {}


void knob_channels::visible_layer_edit(bool visible)
{
    accept_button->setVisible(visible);
    cancel_button->setVisible(visible);
    edit->setVisible(visible);
}

void knob_channels::accept()
{
    QString name = edit->text().replace(" ", "_");
    layer_struct layer;

    layer.name = name;
    layer.red = false;
    layer.green = false;
    layer.blue = true;
    layer.alpha = true;

    glob->layers.push_back(layer);

    update_layers();
    visible_layer_edit(false);
}

void knob_channels::cancel()
{
    visible_layer_edit(false);
}

void knob_channels::update_layers()
{
    // actualiza todas las capas que estan en 'layers' en global
    // y las agrega al combo_box de 'layers'
    for (auto layer : glob->layers)
    {
        combo_box_item item;

        item.value = layer.name;
        item.label = layer.name + " : rgba";
        item.button = false;

        layers->add_item(item);
    }
}
