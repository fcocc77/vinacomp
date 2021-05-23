#include <node_gui.h>
#include <trim_panel.h>
#include <vinacomp.h>

node_gui::node_gui()
    : _trim_panel(nullptr)
    , params(nullptr)
    , _vinacomp(nullptr)
    , _renderer(nullptr)
    , project(nullptr)
{
}

node_gui::~node_gui() {}

void node_gui::setup_env(QWidget *__trim_panel, QWidget *_vinacomp,
                     QJsonObject *_params, QJsonObject _knob_data,
                     QString _name)
{
    _trim_panel = __trim_panel;
    name = _name;
    params = _params;
    knob_data = _knob_data;
    vinacomp *__vinacomp = static_cast<vinacomp *>(_vinacomp);

    _renderer = __vinacomp->get_renderer();
    project = __vinacomp->get_project();
    _project_settings = __vinacomp->get_project_settings();
}

knob *node_gui::get_knob(QString name) const
{
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);
    return panel->get_knob(name);
}

QJsonValue node_gui::get_param_value() const
{
    if (params->contains(name))
        return params->value(name);
    else
        return get_default();
}

void node_gui::setup_knobs() {}

void node_gui::changed(knob *_knob) {}

void node_gui::update_value(QJsonValue value)
{
    if (!params || !_vinacomp)
        return;

    if (get_default() != value)
        params->insert(name, value);
    else
        params->remove(name);

    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}
