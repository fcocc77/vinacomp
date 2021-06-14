#include <animation.h>
#include <curve_editor.h>
#include <project_struct.h>
#include <tools.h>
#include <viewer.h>
#include <vinacomp.h>

curve_editor ::curve_editor(QWidget *__vinacomp)
    : _vinacomp(__vinacomp)
{
    this->setObjectName("curve_editor");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    // Curve Tree
    knobs_tree = new curve_tree();

    connect(knobs_tree, &curve_tree::outside_clicked, this,
            [=]() { view->clear(); });

    connect(knobs_tree, &curve_tree::clicked, this,
            [=](QString node_name, QList<QString> params_name) {
                view->clear();

                for (QString param_name : params_name)
                {
                    knob *_knob = get_knob(node_name, param_name);
                    if (_knob)
                        update_curve(_knob);
                }
            });

    layout->addWidget(knobs_tree);
    // Curve Tree

    // Curve View
    project_struct *project = static_cast<vinacomp *>(_vinacomp)->get_project();

    view = new curve_view(project);

    connect(view, &curve_view::change_curve, this,
            [=](curve *_curve) { update_param(_curve); });

    connect(view, &curve_view::change_frame, this,
            [=](int frame) { update_viewers(frame); });

    connect(view, &curve_view::delete_keyframes, this,
            &curve_editor::delete_keyframes);

    view->setObjectName("graphics_view");
    // Curve View

    QLineEdit *expression_curve = new QLineEdit();
    QLabel *expression_result = new QLabel(" = 20");
    QLabel *expression_name = new QLabel("Transform.translate.x");

    tools *toolbar = new tools();
    toolbar->add_action(view->select_all_action);
    toolbar->add_action(view->delete_keyframes_action);
    toolbar->add_separator();
    toolbar->add_action(view->center_action);
    toolbar->add_stretch();
    toolbar->add_widget(expression_name);
    toolbar->add_widget(expression_curve);
    toolbar->add_widget(expression_result);
    toolbar->add_stretch();
    toolbar->add_action(view->linear_action);
    toolbar->add_action(view->horizontal_action);
    toolbar->add_action(view->smooth_action);
    toolbar->add_action(view->break_action);

    QWidget *view_widget = new QWidget();
    QVBoxLayout *view_layout = new QVBoxLayout(view_widget);
    view_layout->setMargin(0);
    view_layout->setSpacing(0);

    view_layout->addWidget(toolbar);
    view_layout->addWidget(view);

    layout->addWidget(view_widget);
    //
}

curve_editor ::~curve_editor() {}

void curve_editor::update_param(curve *_curve)
{
    QStringList fullname = _curve->get_name().split('.');
    QString node_name = fullname[0];
    QString param_name = fullname[1];

    knob *_knob = get_knob(node_name, param_name);
    if (!_knob)
        return;

    _knob->set_curve(anim::curve_to_string(_curve));
}

void curve_editor::delete_keyframes(QMap<QString, QList<int>> curves)
{
    for (QString curve_name : curves.keys())
    {
        auto indexs_to_delete = curves[curve_name];

        QStringList fullname = curve_name.split('.');
        QString node_name = fullname[0];
        QString param_name = fullname[1];

        knob *_knob = get_knob(node_name, param_name);
        if (!_knob)
            continue;

        QString curve = _knob->get_curve();

        _knob->set_curve(anim::delete_keys_from_curve(curve, indexs_to_delete));
        update_curve(_knob);
    }
}

knob *curve_editor::get_knob(QString node_name, QString param_name)
{
    trim_panel *panel = panels.value(node_name);

    knob *_knob = nullptr;
    if (panel)
        _knob = panel->get_knob(param_name);

    return _knob;
}

void curve_editor::update_from_trim_panel(trim_panel *panel)
{
    delete_panel(panel);

    for (QString key : panel->get_knobs()->keys())
    {
        knob *_knob = panel->get_knobs()->value(key);
        if (!_knob)
            continue;
        if (!_knob->is_animated())
            continue;

        update_panel(panel, _knob->get_name());
    }
}

void curve_editor::update_panel(trim_panel *panel, QString param_name)
{
    QString node_name = panel->get_name();
    QString dimension = "r";

    knobs_tree->add_item(node_name, param_name, dimension, panel->get_color());

    if (!panels.contains(node_name))
        panels.insert(node_name, panel);
}

void curve_editor::update_curve(knob *_knob)
{
    QString param_name = _knob->get_name();
    QString node_name = _knob->get_node_name();

    QString curve_name = node_name + '.' + param_name;
    QString curve = _knob->get_curve();

    update_panel(static_cast<trim_panel *>(_knob->get_panel()), param_name);

    view->delete_curve(curve_name);
    view->create_curve(curve_name, Qt::cyan, anim::convert_curve(curve));
}

void curve_editor::delete_node_item(QString node_name)
{
    knobs_tree->delete_item(node_name);
    panels.remove(node_name);
}

void curve_editor::delete_panel(trim_panel *panel)
{
    QString node_name = panel->get_name();
    delete_node_item(node_name);

    for (QString key : panel->get_knobs()->keys())
    {
        knob *_knob = panel->get_knobs()->value(key);
        if (!_knob)
            continue;
        if (!_knob->is_animated())
            continue;

        QString curve_name = node_name + '.' + _knob->get_name();
        view->delete_curve(curve_name);
    }
}

void curve_editor::update_viewers(int frame)
{
    auto viewers = static_cast<vinacomp *>(_vinacomp)->get_viewers();
    for (viewer *_viewer : *viewers)
        _viewer->get_time_line()->go_to_frame(frame, true);
}
