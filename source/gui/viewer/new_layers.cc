#include <new_layers.h>

new_layers::new_layers(global *_glob, combo_box *_layers)
    : glob(_glob)
    , layers(_layers)
{
    this->setObjectName("new_layer");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 20, 0);

    edit = new QLineEdit;
    edit->setMaximumWidth(100);

    accept_button = new button();
    connect(accept_button, &QPushButton::clicked, this, &new_layers::accept);

    cancel_button = new button();
    connect(cancel_button, &QPushButton::clicked, this, &new_layers::cancel);

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

    layout->addWidget(edit);

    layout->addWidget(red);
    layout->addWidget(green);
    layout->addWidget(blue);
    layout->addWidget(alpha);

    layout->addWidget(accept_button);
    layout->addWidget(cancel_button);

    this->hide();
}

new_layers::~new_layers() {}

void new_layers::add_to_combo_box(combo_box *combo)
{
    // tambien sirve para añadir las capas a los combo_box de los nodos
    for (auto layer : glob->layers)
    {
        combo_box_item item;

        item.value = layer.name;
        item.label = layer.name + ":4";

        combo->add_item(item);
    }
}

void new_layers::accept()
{
    QString name = edit->text().replace(" ", "_");
    layer_struct layer;

    layer.name = name;
    layer.red = false;
    layer.green = false;
    layer.blue = true;
    layer.alpha = true;

    glob->layers.push_back(layer);

    add_to_combo_box(layers);

    this->hide();
}

void new_layers::cancel()
{
    this->hide();
}
