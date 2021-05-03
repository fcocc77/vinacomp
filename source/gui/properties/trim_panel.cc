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
    , knob_editor_visible(false)
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

    knob_editor_container = new QWidget(this);
    layout->addWidget(knob_editor_container);

    tabs = tabs_ui();
    layout->addWidget(tabs);

    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);

    // curve_editor para emitir señal al editor de curva
    _curve_editor = vina->get_curve_editor();

    // obtiene la lista de viewers en una lista de viewers pero con 'QWidget'
    // para usarlos con static_cast y no tener que importar el viewer.h a cada knob
    viewers_gl = vina->get_viewers_gl();
    //

    QJsonArray _knobs = nodes_loaded->get_effect(type).value("knobs").toArray();
    setup_gui_panels(_knobs);
    update_controls_knobs(_knobs);

    QJsonArray shared_knobs =
        jread("source/engine/nodes/json/shared_params.json").value("knobs").toArray();
    setup_knobs(shared_knobs, node_tab_layout, viewers_gl);
    setup_shared_params();

    // si no existen 'knobs' oculta el tab de 'control'
    if (_knobs.empty())
    {
        // tabs->set_index(1);
        // tabs->get_tab(0)->setVisible(false);
    }
}

trim_panel::~trim_panel()
{
    delete layout;
    delete knob_editor_container;

    if (_node_gui)
        delete _node_gui;

    // knobs
    for (knob *_knob : *knobs)
        delete _knob;
    delete knobs;
    //

    delete buttons;

    // tabs
    delete node_tab_layout;
    delete node_tab;

    delete controls_layout;
    delete controls_tab;

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
    setup_knobs(_knobs, controls_layout, viewers_gl);
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

void trim_panel::setup_gui_panels(QJsonArray _knobs)
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
        _node_gui = new shuffle_gui(controls_layout);
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

    knob_editor_button = new QPushButton(widget);
    connect(knob_editor_button, &QPushButton::clicked, this, [=]() { knob_editor_toggle(); });
    qt::set_icon(knob_editor_button, "edit_a", icon_size);
    layout->addWidget(knob_editor_button);

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

tab_widget *trim_panel::tabs_ui()
{
    tab_widget *tabs = new tab_widget();
    tabs->setParent(this);

    controls_tab = new QWidget(this);
    controls_layout = new QVBoxLayout(controls_tab);
    controls_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    controls_layout->setSpacing(5);
    controls_tab->setObjectName("controls");

    node_tab = new QWidget(this);
    node_tab->setObjectName("node_tab");
    node_tab_layout = new QVBoxLayout(node_tab);
    node_tab_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    node_tab_layout->setSpacing(5);

    tabs->add_tab(controls_tab, "Controls");
    tabs->add_tab(node_tab, "Node");

    tabs->set_index(0);

    return tabs;
}

void trim_panel::knob_editor_toggle()
{
    if (!_knob_editor)
    {
        // el 'knob_editor' solo se crea cuando se presiona el boton se 'edit', ya que
        // solo se usa para editar los knobs, y solo se crea si no esta creado antes
        QHBoxLayout *knob_editor_layout = new QHBoxLayout(knob_editor_container);
        knob_editor_layout->setMargin(0);
        _knob_editor = new knob_editor(this);
        _knob_editor->hide();
        knob_editor_layout->addWidget(_knob_editor);
    }

    knob_editor_visible = !knob_editor_visible;
    _knob_editor->setVisible(knob_editor_visible);

    if (knob_editor_visible)
        qt::set_icon(knob_editor_button, "edit_c", 20);
    else
        qt::set_icon(knob_editor_button, "edit_a", 20);
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

