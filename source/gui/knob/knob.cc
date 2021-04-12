#include <action.h>
#include <animation.h>
#include <knob.h>
#include <qt.h>
#include <trim_panel.h>
#include <viewer_gl.h>
#include <vinacomp.h>

knob::knob()
    : knob_layout(nullptr)
    , animation_button(nullptr)
    , _vinacomp(nullptr)
    , params(nullptr)
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

void knob::set_env(QWidget *__parent, project_struct *_project,
                   QWidget *__vinacomp, QList<QWidget *> *_viewers_gl)
{
    _parent = __parent;
    project = _project;
    _vinacomp = __vinacomp;
    viewers_gl = _viewers_gl;
}

QString knob::get_node_name() const
{
    return static_cast<trim_panel *>(_parent)->get_name();
}

QString knob::get_node_type() const
{
    return static_cast<trim_panel *>(_parent)->get_type();
}

void knob::set_data(QJsonObject _knob_data, QJsonObject *_params)
{
    knob_data = _knob_data;
    params = _params;

    // animatable
    bool animatable = true;
    if (knob_data.contains("animatable"))
        animatable = knob_data.value("animatable").toBool();

    set_animatable(animatable);
    //

    // visible
    bool visible = true;
    if (knob_data.contains("visible"))
        visible = knob_data.value("visible").toBool();

    set_visible(visible);
    //

    // name and type
    name = knob_data.value("name").toString();
    type = knob_data.value("type").toString();
    anim_name = name + "_anim";
    //
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
    action *curve_editor_action =
        new action("Curve Editor...", "", "curve_editor");
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
        enable_animation();
        set_animated(true);
    });
    delete_key_action->connect_to(this, [=]() {});
    no_animation_action->connect_to(this, [=]() {
        disable_animation();
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

QJsonValue knob::get_param_value() const
{
    if (params->contains(name))
        return params->value(name);
    else
        return get_default();
}

void knob::restore_param()
{
    bool _animated = false;

    if (params->contains(anim_name))
        _animated = params->value(anim_name).toBool();

    if (_animated)
    {
        enable_animation();
        set_animated(true);
    }
}

void knob::update_value(QJsonValue value)
{
    if (!params || !_vinacomp)
        return;

    if (!animated)
    {
        if (get_default() != value)
        {
            params->insert(name, value);
            params->insert(anim_name, false);
        }
        else
        {
            params->remove(name);
            params->remove(anim_name);
        }
    }
    else
    {
        params->insert(anim_name, true);
        set_keyframe(false);
        // params->insert(name,
        // anim::update_curve(params->value(name).toString(), _value,
        // project->frame));
    }

    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}

void knob::enable_animation()
{
    animated = true;
    set_keyframe();
}

void knob::disable_animation()
{
    // el 'set_animated' solo funciona como virtual para las subclases,
    // y 'enable_animation' y 'disable_animation' se usan aqui ya que estas
    // intervienen el 'params' del proyecto, y cuando se usa el 'restore_param'
    // da conflicto ya que guarda el proyecto antes de lo necesario.
    // ! no unir estas 2'
    animated = false;

    QString curve = get_param_value().toString();
    float value = anim::get_value(curve, project->frame);
    update_value(value);
}

void knob::set_animated(bool _animated) {}

void knob::set_keyframe(bool auto_value)
{
    QString curve = get_param_value().toString();
    QString new_curve;

    if (curve.isEmpty() || !auto_value)
        new_curve = anim::set_keyframe(curve, project->frame, false,
                                       get_param_value().toDouble());
    else
        new_curve = anim::set_keyframe(curve, project->frame);

    params->insert(name, new_curve);
    params->insert(anim_name, true);

    curve_editor *_curve_editor =
        static_cast<vinacomp *>(_vinacomp)->get_curve_editor();

    _curve_editor->update_curve(this);
}
