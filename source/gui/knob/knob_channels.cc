#include <knob_channels.h>

knob_channels::knob_channels(project_struct *_project, QString _layer,
                             QList<bool> _channels)
    : current_layer(_layer)
    , project(_project)
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
    connect(cancel_button, &QPushButton::clicked, this, [this]() {
        set_layer(current_layer);
        visible_layer_edit(false);
    });

    accept_button->setText("OK");
    cancel_button->setText("Cancel");

    red = new knob_check_box("", _channels[0]);
    red->setObjectName("red");

    green = new knob_check_box("", _channels[1]);
    green->setObjectName("green");

    blue = new knob_check_box("", _channels[2]);
    blue->setObjectName("blue");

    alpha = new knob_check_box("", _channels[3]);
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
    connect(layers, &combo_box::changed, this, [=](QVariant value) {
        current_layer = value.toString();
        set_layer(current_layer);
        to_emmit_signal();
    });
    connect(layers, &combo_box::pre_open, this, [=]() { update_layers(); });
    update_layers("", true);
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

void knob_channels::set_layer(QString layer_name)
{
    bool _red = true;
    bool _green = true;
    bool _blue = true;
    bool _alpha = true;

    auto *layer = get_layer(layer_name);
    // si la capa no existe en las 'layers' puede ser
    // que sea la 'layer' principal 'main'
    if (layer)
    {
        _red = layer->red;
        _green = layer->green;
        _blue = layer->blue;
        _alpha = layer->alpha;
    }

    red->setVisible(_red);
    green->setVisible(_green);
    blue->setVisible(_blue);
    alpha->setVisible(_alpha);
}

void knob_channels::to_emmit_signal()
{
    changed(current_layer, {red->is_checked(), green->is_checked(),
                            blue->is_checked(), alpha->is_checked()});
}

layer_struct *knob_channels::get_layer(QString layer_name)
{
    for (auto &layer : project->global.layers)
        if (layer.name == layer_name)
            return &layer;

    return nullptr;
}

void knob_channels::set_all_channel(bool value)
{
    if (qt::control())
    {
        red->set_check(value, false);
        green->set_check(value, false);
        blue->set_check(value, false);
        alpha->set_check(value, false);
    }

    to_emmit_signal();
}

void knob_channels::visible_layer_edit(bool visible)
{
    if (visible)
    {
        red->show();
        green->show();
        blue->show();
        alpha->show();
    }

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

    for (auto lay : project->global.layers)
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

    project->global.layers.push_back(layer);

    update_layers("add");
    visible_layer_edit(false);
    edit->clear();
}

void knob_channels::edit_layer()
{
    QString name = checking();
    if (name.isEmpty())
        return;

    auto *layer = get_layer(layer_to_edit);

    layer->name = name;
    layer->red = red->is_checked();
    layer->green = green->is_checked();
    layer->blue = blue->is_checked();
    layer->alpha = alpha->is_checked();

    update_layers("edit");
    visible_layer_edit(false);
    edit->clear();
}

void knob_channels::update_layers(QString from, bool keep_current_layer)
{
    int old_index = layers->get_index();
    QString old_layer = layers->get_value().toString();

    layers->clear();
    layers->add_item({"main : rgba", "main"});

    // actualiza todas las capas que estan en 'layers' en global
    // y las agrega al combo_box de 'layers'
    for (auto layer : project->global.layers)
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
        layer_to_edit = old_layer;
        layer_to_edit_index = old_index;
        visible_layer_edit(true);
    });

    int index_new = layers->add_item({"New", "new", true, "add"});
    layers->get_action(index_new)->connect_to(this, [=]() {
        action = "new";
        layer_to_edit = "";
        edit->clear();
        visible_layer_edit(true);
    });

    if (keep_current_layer)
    {
        layers->set_value(current_layer, false);
        return;
    }

    // si la actualizacion viene de 'add_layer' deja el index
    // de la nueva capa, y envia la señal para renderizar, y "edit_layer"
    if (from == "add")
        layers->set_index(layers->count() - 3, true);
    else if (from == "edit")
        layers->set_index(layer_to_edit_index, true);
    else
        layers->set_index(old_index, false);
}
