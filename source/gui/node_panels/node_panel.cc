#include <node_panel.h>
#include <trim_panel.h>
#include <vinacomp.h>

node_panel::node_panel()
    : _trim_panel(nullptr)
    , params(nullptr)
    , _vinacomp(nullptr)
    , _renderer(nullptr)
    , project(nullptr)
{
}

node_panel::~node_panel() {}

void node_panel::setup_env(QWidget *__trim_panel, QWidget *___vinacomp,
                         QJsonObject *_params, QJsonObject _knob_data,
                         QString _name, QWidget *__node_view,
                         QGraphicsItem *_this_node)
{
    _vinacomp = ___vinacomp;

    _trim_panel = __trim_panel;
    name = _name;
    params = _params;
    knob_data = _knob_data;
    vinacomp *__vinacomp = static_cast<vinacomp *>(_vinacomp);
    _node_view = __node_view;
    this_node = _this_node;

    _renderer = __vinacomp->get_renderer();
    project = __vinacomp->get_project();
    _project_settings = __vinacomp->get_project_settings();
}

knob *node_panel::get_knob(QString name) const
{
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);
    return panel->get_knob(name);
}

QJsonValue node_panel::get_param_value() const
{
    if (params->contains(name))
        return params->value(name);
    else
        return get_default();
}

void node_panel::setup_knobs(QMap<QString, QVBoxLayout *> layouts) {}

void node_panel::close() {}

void node_panel::changed(knob *_knob) {}

QString node_panel::get_script() const
{
    return "";
}

void node_panel::update_value(QJsonValue value)
{
    if (!params || !_vinacomp)
        return;

    if (get_default() != value)
        params->insert(name, value);
    else
        params->remove(name);

    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}
