#include <knob_channels.h>

knob_channels::knob_channels(global *_glob)
    : glob(_glob)
{
    this->setObjectName("knob_channels");

    layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);
    layout->setMargin(0);

    edit = new QLineEdit;
    edit->setMaximumHeight(20);
    edit->hide();

    accept_button = new button();
    accept_button->hide();
    connect(accept_button, &QPushButton::clicked, this, [this]() {
        if (action == "edit")
            edit_layer();
        else
            add_layer();
    });

    cancel_button = new button();
    cancel_button->hide();
    connect(cancel_button, &QPushButton::clicked, this,
            [this]() { visible_layer_edit(false); });

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

    connect(red, &knob_check_box::changed, this,
            &knob_channels::set_all_channel);
    connect(green, &knob_check_box::changed, this,
            &knob_channels::set_all_channel);
    connect(blue, &knob_check_box::changed, this,
            &knob_channels::set_all_channel);
    connect(alpha, &knob_check_box::changed, this,
            &knob_channels::set_all_channel);

    // Layers
    layers = new combo_box();
    connect(layers, &combo_box::changed, this, [=](QVariant value, int index) {
        changed(value.toString(), index);
    });
    connect(layers, &combo_box::pre_open, this, [=]() { update_layers(); });
    update_layers();
    //

    layout->addWidget(init_space);

    layout->addWidget(layers);

    layout->addWidget(red);
    layout->addWidget(green);
    layout->addWidget(blue);
    layout->addWidget(alpha);

    layout->addWidget(edit);

    layout->addWidget(accept_button);
    layout->addWidget(cancel_button);
}

knob_channels::~knob_channels() {}

void knob_channels::set_all_channel(bool value)
{
    if (qt::control())
    {
        red->set_check(value, false);
        green->set_check(value, false);
        blue->set_check(value, false);
        alpha->set_check(value, false);
    }
}

void knob_channels::visible_layer_edit(bool visible)
{
    accept_button->setVisible(visible);
    cancel_button->setVisible(visible);
    edit->setVisible(visible);
    edit->setFocus();
}

QString knob_channels::checking()
{
    QString name = edit->text().replace(" ", "_");

    if (name.isEmpty())
    {
        QString msg = "No name";
        QMessageBox::warning(this, "VinaComp Error", msg, QMessageBox::Ok);
        return "";
    }

    for (auto lay : glob->layers)
    {
        if (lay.name == name || name == "main")
        {
            if (name != layer_to_edit)
            {
                QString msg = "Layer: '" + name + "' exist.";
                QMessageBox::warning(this, "VinaComp Error", msg,
                                     QMessageBox::Ok);
                return "";
            }
        }
    }

    if (!red->is_checked() && !green->is_checked() && !blue->is_checked() &&
        !alpha->is_checked())
    {
        QString msg = "Must have at least 1 channel selected";
        QMessageBox::warning(this, "VinaComp Error", msg, QMessageBox::Ok);
        return "";
    }

    return name;
}

void knob_channels::add_layer()
{
    QString name = checking();
    if (name.isEmpty())
        return;

    layer_struct layer;

    layer.name = name;
    layer.red = red->is_checked();
    layer.green = green->is_checked();
    layer.blue = blue->is_checked();
    layer.alpha = alpha->is_checked();

    glob->layers.push_back(layer);

    update_layers("add");
    visible_layer_edit(false);
    edit->clear();
}

void knob_channels::edit_layer()
{
    QString name = checking();
    if (name.isEmpty())
        return;

    for (auto &layer : glob->layers)
    {
        if (layer.name == layer_to_edit)
        {
            layer.name = name;
            layer.red = red->is_checked();
            layer.green = green->is_checked();
            layer.blue = blue->is_checked();
            layer.alpha = alpha->is_checked();

            break;
        }
    }

    update_layers("edit");
    visible_layer_edit(false);
    edit->clear();
}

void knob_channels::update_layers(QString from)
{
    int current_index = layers->get_index();
    QString current_layer = layers->get_value().toString();

    layers->clear();
    layers->add_item({"main : rgba", "main"});

    // actualiza todas las capas que estan en 'layers' en global
    // y las agrega al combo_box de 'layers'
    for (auto layer : glob->layers)
    {
        combo_box_item item;

        QString channels;
        if (layer.red)
            channels += "r";
        if (layer.green)
            channels += "g";
        if (layer.blue)
            channels += "b";
        if (layer.alpha)
            channels += "a";

        item.value = layer.name;
        item.label = layer.name + " : " + channels;
        item.button = false;

        layers->add_item(item);
    }

    layers->add_separator();

    int index_edit = layers->add_item({"Edit", "edit", true, "edit"});
    layers->get_action(index_edit)->connect_to(this, [=]() {
        if (current_layer == "main")
        {
            QString msg = "main layer cannot be edited.";
            QMessageBox::warning(this, "VinaComp Error", msg, QMessageBox::Ok);
            return;
        }

        action = "edit";
        edit->setText(current_layer);
        layer_to_edit = current_layer;
        layer_to_edit_index = current_index;
        visible_layer_edit(true);
    });

    int index_new = layers->add_item({"New", "new", true, "add"});
    layers->get_action(index_new)->connect_to(this, [=]() {
        action = "new";
        layer_to_edit = "";
        edit->clear();
        visible_layer_edit(true);
    });

    // si la actualizacion viene de 'add_layer' deja el index
    // de la nueva capa, y envia la señal para renderizar, y "edit_layer"
    if (from == "add")
        layers->set_index(layers->count() - 3, true);
    else if (from == "edit")
        layers->set_index(layer_to_edit_index, true);
    else
        layers->set_index(current_index, false);
}
