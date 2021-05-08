#include <knob_group.h>

knob_group::knob_group(knob_props props, QString label, int _knobs_included,
                       bool open_group)
    : knob(props)
    , knobs_included(_knobs_included)
    , open(open_group)
{
    this->setObjectName("knob_group");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 10, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(init_space);

    label_widget = new QLabel(this);
    label_widget->setText(label);

    layout->addWidget(label_widget);

    arrow_button = new QPushButton(this);
    connect(arrow_button, &QPushButton::clicked, this, [=]() { set_open(!open); });
    arrow_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    qt::set_icon(arrow_button, "arrow_down_a", 18);
    init_space->layout()->addWidget(arrow_button);
}

knob_group::~knob_group() {}

void knob_group::restore_param()
{
    knob::restore_param();

    // int knobs_included = knob_object.value("knobs").toInt();
    // bool open_group = knob_object.value("open").toBool();
}

void knob_group::update()
{
    set_open(open);
}

void knob_group::set_open(bool _open)
{
    open = _open;

    for (QWidget *knob : get_knobs_included())
        knob->setVisible(open);

    qt::set_property(label_widget, "open", open);
    qt::set_property(init_space, "open", open);

    // cambio de icono de flecha
    if (open)
        qt::set_icon(arrow_button, "arrow_down_a", 18);
    else
        qt::set_icon(arrow_button, "arrow_right_a", 18);
}

QList<QWidget *> knob_group::get_knobs_included()
{
    // obtiene todos los 'knobs' incluidos en este grupo,
    // el metodo usado es obtener el padre, e iterar todos los hijos,
    // al encontrar este mismo grupo se suman los widget siguientes.

    QWidget *parent = this->parentWidget();
    bool this_group = false;

    QList<QWidget *> knobs;
    int knobs_added = 0;
    for (int i = 0; i < parent->layout()->count(); i++)
    {
        QWidget *widget = parent->layout()->itemAt(i)->widget();

        if (this_group)
        {
            if (knobs_added >= knobs_included)
                break;
            knobs.push_back(widget);
            knobs_added++;
        }

        if (widget == this)
            this_group = true;
    }

    return knobs;
}

void knob_group::mousePressEvent(QMouseEvent *event)
{
    set_open(!open);
}
