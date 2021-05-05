#include <trim_panel.h>
#include <vinacomp.h>
#include <shuffle_gui.h>
#include <reformat_gui.h>
#include <frame_range_gui.h>
#include <write_gui.h>
#include <knob_text.h>
#include <knob_check_box.h>
#include "node.h"
#include <node_backdrop.h>
#include <node_view.h>

trim_panel::trim_panel(properties *__properties, QString _name, QString _type,
                       QColor _color, QString _icon_name,
                       nodes_load *_nodes_loaded, project_struct *_project,
                       QWidget *__vinacomp, QGraphicsItem *_node,
                       QWidget *__node_view)

    : _knob_editor(nullptr)
    , this_node(_node)
    , nodes_loaded(_nodes_loaded)
    , _vinacomp(__vinacomp)
    , project(_project)
    , _node_view(__node_view)

    , is_maximize(true)
    , _properties(__properties)
    , _node_gui(nullptr)
    , name(_name)
    , type(_type)
    , icon_name(_icon_name)
    , color(_color)
    , data(project->nodes[_name].params)

{
    knobs = new QMap<QString, knob *>;

    this->setObjectName("trim_panel");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setSpacing(0);
    layout->setMargin(0);

    buttons = top_buttons_setup_ui();
    layout->addWidget(buttons);

    // obtiene la lista de viewers en una lista de viewers pero con 'QWidget'
    // para usarlos con static_cast y no tener que importar el viewer.h a cada
    // knob
    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);
    viewers_gl = vina->get_viewers_gl();

    // tabs = tabs_ui();
    _knobs = nodes_loaded->get_effect(type).value("knobs").toArray();

    // encuentra todos los tab que estan en los 'knobs'
    QStringList finded_tabs;
    for (QJsonValue value : _knobs)
    {
        QString tab_name = value.toObject().value("tab").toString();
        if (!tab_name.isEmpty() && !finded_tabs.contains(tab_name))
            finded_tabs.push_back(tab_name);
    }
    //

    tabs = new tab_widget();
    layout->addWidget(tabs);

    // añade cada tabs, 'controls' y 'node' son por defecto
    add_tab("controls");
    for (QString tab_name : finded_tabs)
        add_tab(tab_name);
    add_tab("node");

    tabs_only_read = finded_tabs;
    tabs_only_read.push_back("controls");
    tabs_only_read.push_back("node");
    //

    tabs->set_index(0);
    //
}

trim_panel::~trim_panel()
{
    delete layout;

    if (_node_gui)
        delete _node_gui;

    // knobs
    for (knob *_knob : *knobs)
        delete _knob;
    delete knobs;
    //

    delete buttons;

    // tabs
    delete tabs;
    //
}

void trim_panel::update_controls_knobs(QJsonArray _knobs)
{
    // !!! hay  que borrar solo los de 'controls_layout' ahora los borra todos
    for (knob *_knob : *knobs)
        delete _knob;
    knobs->clear();

    // borrar todos los que knobs antes de agregar los nuevos
    // setup_knobs(_knobs, controls_layout, viewers_gl);
}

void trim_panel::set_edit_mode(bool enable)
{
    for (tab *_tab : tabs->get_tabs())
        if (!tabs_only_read.contains(_tab->get_name()))
            _tab->set_visible_close_button(enable);
}

void trim_panel::setup_shared_params()
{
    // crea las conecciones a los parametros que estan en el tab de 'node'
    node *_this_node = static_cast<node *>(this_node);

    knob_text *_knob_text = static_cast<knob_text *>(get_knob("label"));
    connect(_knob_text, &knob_text::text_changed, this, [=](QString text) {
        _this_node->set_tips(text);
        _this_node->refresh();
    });

    knob_check_box *disable_node_knob =
        static_cast<knob_check_box *>(get_knob("disable_node"));
    connect(disable_node_knob, &knob_check_box::changed, this,
            [=](bool changed) {
                static_cast<node_rect *>(_this_node)->set_disable(changed);
            });

    if (type == "backdrop")
        get_knob("disable_node")->set_visible(false);
}

void trim_panel::setup_gui_panels(QJsonArray _knobs, QVBoxLayout *_layout)
{
    // todos estos nodo gui son solo si el nodo efecto tiene algun boton
    // u otra interface adicional a las que se generan en 'setup_knobs'
    // por eso solo son algunos nodos y no todos
    QJsonObject knob_data;

    if (type == "frame_range")
        _node_gui = new frame_range_gui();
    else if (type == "reformat")
        _node_gui = new reformat_gui();
    else if (type == "write")
        _node_gui = new write_gui(project);
    else if (type == "shuffle")
    {
        knob_data = _knobs[0].toObject();
        _node_gui = new shuffle_gui(_layout);
    }

    if (_node_gui)
    {
        _node_gui->setup(this, _vinacomp, data, knob_data, name);
        _node_gui->restore_param();
    }
}

QWidget *trim_panel::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("butttons");
    widget->setMaximumHeight(30);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setMargin(5);
    //
    //

    int icon_size = 20;

    QPushButton *center_node = new QPushButton(widget);
    qt::set_icon(center_node, "center_a", icon_size);
    layout->addWidget(center_node);

    restart_button = new QPushButton(widget);
    connect(restart_button, &QPushButton::clicked, this, [=]() {  });
    qt::set_icon(restart_button, "restart_a", icon_size);
    layout->addWidget(restart_button);

    layout->addStretch();

    QPushButton *icon_node = new QPushButton(widget);
    if (icon_name.contains("/"))
        qt::set_icon(icon_node, icon_name, icon_size);
    else
        qt::set_icon(icon_node, icon_name + "_a", icon_size);
    layout->addWidget(icon_node);

    name_edit = new QLineEdit(widget);
    connect(name_edit, &QLineEdit::editingFinished, [=]() {
        if (name_edit->text() == name)
            return;

        node *_node = static_cast<node *>(this_node);
        bool renamed = static_cast<node_view *>(_node_view)
                           ->rename_node(_node, name_edit->text());
        if (!renamed)
            name_edit->setText(name);
    });
    name_edit->setText(name);
    layout->addWidget(name_edit);

    layout->addStretch();

    // Minimize
    QPushButton *minimize = new QPushButton(widget);
    connect(minimize, &QPushButton::clicked, this, [=]() { this->maximize(!is_maximize); });
    qt::set_icon(minimize, "minimize_a", icon_size);
    layout->addWidget(minimize);
    // Minimize
    //

    QPushButton *maximize_button = new QPushButton(widget);
    qt::set_icon(maximize_button, "maximize_a", icon_size);
    layout->addWidget(maximize_button);

    QPushButton *close = new QPushButton(widget);
    connect(close, &QPushButton::clicked, this,
            [this]() { _properties->close_trim_panel(this->get_name()); });
    qt::set_icon(close, "close_a", icon_size);
    layout->addWidget(close);

    return widget;
}

void trim_panel::add_tab(QString tab_name, int index)
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setSpacing(5);
    widget->setObjectName("knobs_tab");

    tabs->add_tab(widget, tab_name, index);

    if (tab_name == "node")
    {
        QJsonArray shared_knobs =
            jread("source/engine/nodes/json/shared_params.json")
                .value("knobs")
                .toArray();
        setup_knobs(shared_knobs, layout, viewers_gl);
        setup_shared_params();

        return;
    }

    QJsonArray this_tab_knobs;
    for (QJsonValue knob : _knobs)
    {
        QString _tab_name = knob.toObject().value("tab").toString();

        // si el tab del knob esta vacio, queda en el tab por defecto que es 'controls'
        if (tab_name == "controls" && _tab_name.isEmpty())
        {
            this_tab_knobs.push_back(knob);
            continue;
        }

        if (_tab_name == tab_name)
            this_tab_knobs.push_back(knob);
    }

    if (tab_name == "controls")
        setup_gui_panels(this_tab_knobs, layout);

    setup_knobs(this_tab_knobs, layout, viewers_gl);
}

void trim_panel::maximize(bool _maximize)
{
    tabs->setVisible(_maximize);
    is_maximize = _maximize;

    for (QWidget *vgl : *viewers_gl)
        static_cast<viewer_gl *>(vgl)->handlers_update();
}

void trim_panel::update_render()
{
    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}

