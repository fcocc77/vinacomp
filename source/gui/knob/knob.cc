#include <action.h>
#include <animation.h>
#include <knob.h>
#include <qt.h>
#include <trim_panel.h>
#include <viewer_gl.h>
#include <vinacomp.h>
#include <node_rect.h>
#include <knob_editor.h>
#include <script_editor.h>
#include <knob_curve_menu.h>

knob::knob(knob_props props)
    : knob_layout(nullptr)
    , animation_button(nullptr)
    , panel(props.panel)
    , this_node(props.this_node)
    , _knob_editor(props._knob_editor)
    , viewers_gl(props.viewers_gl)
    , edit_mode(false)
    , linked(false)
    , _vinacomp(props._vinacomp)
    , over_line_widget(nullptr)
    , knob_data(props.knob_data)
    , params(props.params)
    , init_space_width(0)
    , project(props.project)
    , animated(false)
{
    // name and type
    name = knob_data.value("name").toString();
    type = knob_data.value("type").toString();
    label = knob_data.value("label").toString();
    tips = knob_data.value("tooltip").toString();
    curve_name = name + "_curve";
    exp_name = name + "_exp";
    linked_name = name + "_linked";
    linked_list_name = name + "_linked_list";
    //

    // Espacio inicial
    init_space = new QWidget();
    init_space->hide();
    init_space->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    init_space->setObjectName("init_space");

    delete_knob_button = new button;
    connect(delete_knob_button, &button::clicked, this, [=]() {
        static_cast<knob_editor *>(_knob_editor)->delete_knob(this);
    });
    delete_knob_button->setFixedSize({25, 22});
    delete_knob_button->hide();
    delete_knob_button->set_icon("close");

    edit_knob_button = new button;
    connect(edit_knob_button, &button::clicked, this, [=]() {
        static_cast<knob_editor *>(_knob_editor)->edit_knob(this);
    });
    edit_knob_button->setFixedSize({25, 22});
    edit_knob_button->hide();
    edit_knob_button->set_icon("edit");

    drag_knob_button = new button;
    connect(drag_knob_button, &button::pressed, this, [=]() {
        static_cast<knob_editor *>(_knob_editor)->drag_knob(this);
    });
    drag_knob_button->setFixedSize({25, 22});
    drag_knob_button->hide();
    drag_knob_button->set_icon("drag");

    QHBoxLayout *layout = new QHBoxLayout(init_space);
    label_widget = new QLabel();
    label_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addWidget(delete_knob_button);
    layout->addWidget(edit_knob_button);
    layout->addWidget(drag_knob_button);
    layout->addStretch();
    layout->addWidget(label_widget);
    layout->setMargin(0);
    //
    //

    icon_size = 15;

    if (tips.isEmpty())
        tips = name;
    else
        tips = name + ":\n\n" + tips;

    label_widget->setToolTip(tips);
}

knob::~knob()
{
    delete animation_button;
    delete delete_knob_button;
    delete edit_knob_button;
    delete drag_knob_button;
}

void knob::set_editing_knob(bool editing)
{
    // ajusta el editando 'knob'
    if (editing)
        edit_knob_button->set_hover_icon();
    else
        edit_knob_button->set_normal_icon();
}

QString knob::get_node_name() const
{
    return static_cast<trim_panel *>(panel)->get_name();
}

QString knob::get_node_type() const
{
    return static_cast<trim_panel *>(panel)->get_type();
}

void knob::set_data()
{
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

void knob::set_init_space(int space)
{
    init_space_width = space;

    if (space == 0)
        return;

    init_space->show();
    init_space->setMaximumWidth(space);
    init_space->setMinimumWidth(space);
}

void knob::set_init_label(bool has_label)
{
    if (has_label)
        label_widget->setText(label);
    else
        label_widget->setText("");
}

void knob::set_init_label_text(QString label)
{
    label_widget->setText(label);
}

void knob::set_edit_mode(bool enable)
{
    edit_mode = enable;

    init_space->show();
    delete_knob_button->setVisible(enable);
    edit_knob_button->setVisible(enable);
    drag_knob_button->setVisible(enable);

    if (enable)
    {
        int space = init_space_width + 100;

        init_space->setMaximumWidth(space);
        init_space->setMinimumWidth(space);
    }
    else
    {
        init_space->setMaximumWidth(init_space_width);
        init_space->setMinimumWidth(init_space_width);
    }
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

    knob_curve_menu *curve_menu =
        static_cast<trim_panel *>(panel)->get_curve_menu();

    connect(animation_button, &button::pressed, this,
            [=]() { curve_menu->set_knob(this); });

    animation_button->setMenu(curve_menu);
}

QJsonValue knob::get_param_value() const
{
    if (!params)
        return {};

    if (params->contains(name))
        return params->value(name);
    else
        return get_default();
}

void knob::restore_param()
{
    if (!params)
        return;

    bool _animated = false;

    if (params->contains(curve_name))
        _animated = params->value(curve_name).toBool();

    if (_animated)
    {
        enable_animation();
        set_animated(true);
    }

    set_expression(params->value(exp_name).toString());

    if (params->contains(linked_name))
    {
        QJsonArray linked = params->value(linked_name).toArray();
        set_linked(linked.at(0).toString(), linked.at(1).toString());
    }

    if (params->contains(linked_list_name))
    {
        linked_knobs = params->value(linked_list_name).toArray();
    }
}

void knob::update_value(QJsonValue value)
{
    if (!params)
        return;

    if (!animated)
    {
        if (get_default() != value)
        {
            params->insert(name, value);
            params->insert(curve_name, false);
        }
        else
        {
            params->remove(name);
            params->remove(curve_name);
        }
    }
    else
    {
        params->insert(curve_name, true);
        set_keyframe(false);
        // params->insert(name,
        // anim::update_curve(params->value(name).toString(), _value,
        // project->frame));
    }

    if (_vinacomp)
        static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}

void knob::enable_animation()
{
    animated = true;
    set_keyframe();
    static_cast<node_rect *>(this_node)->set_animated(true);
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

    static_cast<node_rect *>(this_node)->set_animated(false);
}

void knob::set_animated(bool _animated) {}

void knob::set_has_expression(bool expression) {}

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
    params->insert(curve_name, true);

    curve_editor *_curve_editor =
        static_cast<vinacomp *>(_vinacomp)->get_curve_editor();

    _curve_editor->update_curve(this);
}

void knob::set_expression(QString expression)
{
    bool has_expression = !expression.isEmpty();

    set_has_expression(has_expression);

    if (has_expression)
    {
        params->insert(exp_name, expression);
    }
    else
    {
        params->remove(exp_name);
        set_error(false);
    }
}

void knob::add_link_knob(QString node_name, QString param_name)
{
    // lista de knobs que son manejados por este knob
    linked_knobs.push_back(QJsonArray{node_name, param_name});
    params->insert(linked_list_name, linked_knobs);
}

void knob::remove_link_knob(QString node_name, QString param_name)
{
    int index = 0;
    bool finded = false;

    for (knob *_knob : get_linked_knobs())
    {
        if (_knob->get_node_name() == node_name &&
            _knob->get_name() == param_name)
        {
            finded = true;
            break;
        }

        index++;
    }

    if (finded)
        linked_knobs.removeAt(index);
}

void knob::set_linked(QString node_name, QString param_name)
{
    // si este knob es manejado por otro knob
    linked = true;

    handler_knob_node_name = node_name;
    handler_knob_name = param_name;

    params->insert(linked_name, QJsonArray{node_name, param_name});
    set_disable(linked);

    static_cast<node_rect *>(this_node)->set_link_item(true);
    static_cast<node *>(this_node)->set_linked(node_name);
}

knob *knob::get_knob(QString node_name, QString param_name) const
{
    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);

    node *_node = vina->get_node_graph()->get_node_view()->get_node(node_name);

    if (!_node)
        for (node_graph *group : *vina->get_groups_node_graph())
        {
            _node = group->get_node_view()->get_node(node_name);
            if (_node)
                break;
        }

    trim_panel *panel = nullptr;
    if (_node)
        panel = _node->get_trim_panel();

    knob *_knob = nullptr;
    if (panel)
        _knob = panel->get_knob(param_name);

    return _knob;
}

QList<knob *> knob::get_linked_knobs() const
{
    QList<knob *> knobs;
    for (QJsonValue value : linked_knobs)
    {
        QString node_name = value.toArray().at(0).toString();
        QString param_name = value.toArray().at(1).toString();

        knob *_knob = get_knob(node_name, param_name);
        if (_knob)
            knobs.push_back(_knob);
    }

    return knobs;
}

void knob::remove_link()
{
    if (!linked)
        return;

    // elimina este knob en el manejador
    knob *handler_knob = get_knob(handler_knob_node_name, handler_knob_name);
    if (handler_knob)
        handler_knob->remove_link_knob(get_node_name(), get_name());

    params->remove(linked_name);
    linked = false;
    set_disable(false);

    static_cast<node_rect *>(this_node)->set_link_item(false);
    static_cast<node *>(this_node)->set_linked(0);
}

void knob::set_disable(bool disable)
{
    qt::set_property(label_widget, "disable", disable);
}

void knob::set_error(bool error) {}

void knob::restore_default()
{
    set_expression("");
    if (animated)
        disable_animation();

    remove_link();

    params->remove(name);
    params->remove(curve_name);
    params->remove(exp_name);
    params->remove(linked_name);
    params->remove(linked_list_name);
}
