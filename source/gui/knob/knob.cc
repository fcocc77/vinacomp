#include <knob.h>
#include <viewer_gl.h>
#include <qt.h>
#include <action.h>

knob::knob()
    : knob_layout(nullptr)
    , animation_button(nullptr)
    , viewers_gl(nullptr)
    , project(nullptr)
    , animated(false)
{
    // Espacio inicial
    init_space = new QWidget();
    init_space->hide();
    init_space->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    init_space->setObjectName("init_space");

    QHBoxLayout *layout = new QHBoxLayout(init_space);
    label_widget = new QLabel();
    layout->addStretch();
    layout->addWidget(label_widget);
    layout->setMargin(0);
    //
    //

    menu = new QMenu(this);

    icon_size = 15;
}

knob::~knob()
{
    delete animation_button;
}

void knob::set_names(QString _node_name, QString _node_type, QString _param_name,
                     QString _param_type)
{
    node_name = _node_name;
    node_type = _node_type;
    name = _param_name;
    type = _param_type;
}

void knob::update_handler()
{
    if (!viewers_gl)
        return;

    for (QWidget *vgl : *viewers_gl)
    {
        viewer_gl *_viewer_gl = static_cast<viewer_gl *>(vgl);
        _viewer_gl->knob_signal(this);
    }
}

void knob::set_init_space(int space, QString label)
{
    if (space == 0)
        return;

    init_space->show();
    init_space->setMaximumWidth(space);
    init_space->setMinimumWidth(space);
    label_widget->setText(label);
}

void knob::update_animated() {}

void knob::set_animatable(bool _animatable)
{
    if (!_animatable)
        return;

    if (animation_button || !knob_layout)
        return;

    animation_button = new button();
    animation_button->setObjectName("small_button");
    animation_button->set_icon("key", icon_size);
    knob_layout->addWidget(animation_button);

    action *set_key_action = new action("Set Key", "", "key");
    action *delete_key_action = new action("Delete Key", "");
    action *no_animation_action = new action("No Animation", "");
    action *curve_editor_action = new action("Curve Editor...", "", "curve_editor");
    action *copy_values_action = new action("Copy Values", "");
    action *copy_animation_action = new action("Copy Animation", "");
    action *copy_links_action = new action("Copy Links", "");
    action *paste_absolute_action = new action("Paste Absolute");
    action *paste_relative_action = new action("Paste Relative");
    action *generate_keys_action = new action("Generate Keys");
    action *smooth_curve_action = new action("Smooth Curve");
    action *loop_action = new action("Loop");

    menu->addAction(set_key_action);
    menu->addAction(delete_key_action);
    menu->addAction(no_animation_action);
    menu->addSeparator();
    menu->addAction(curve_editor_action);
    menu->addSeparator();
    menu->addAction(copy_values_action);
    menu->addAction(copy_animation_action);
    menu->addAction(copy_links_action);
    menu->addSeparator();
    menu->addAction(paste_absolute_action);
    menu->addAction(paste_relative_action);
    menu->addSeparator();
    menu->addAction(generate_keys_action);
    menu->addAction(smooth_curve_action);
    menu->addAction(loop_action);

    animation_button->setMenu(menu);

    // Conecciones
    set_key_action->connect_to(this, [=]() {
        set_animated(true);
        key_frame_changed(true);
    });
    delete_key_action->connect_to(this, [=]() { key_frame_changed(false); });
    no_animation_action->connect_to(this, [=]() {
        set_animated(false);
    });
    curve_editor_action->connect_to(this, [=]() {});
    copy_values_action->connect_to(this, [=]() {});
    copy_animation_action->connect_to(this, [=]() {});
    copy_links_action->connect_to(this, [=]() {});
    paste_absolute_action->connect_to(this, [=]() {});
    paste_relative_action->connect_to(this, [=]() {});
    generate_keys_action->connect_to(this, [=]() {});
    smooth_curve_action->connect_to(this, [=]() {});
    loop_action->connect_to(this, [=]() {});
}
