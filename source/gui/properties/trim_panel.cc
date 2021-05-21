#include <trim_panel.h>
#include <vinacomp.h>
#include <shuffle_gui.h>
#include <reformat_gui.h>
#include <frame_range_gui.h>
#include <group_gui.h>
#include <write_gui.h>
#include <roto_gui.h>
#include <knob_text.h>
#include <knob_color.h>
#include <knob_check_box.h>
#include "node.h"
#include <node_backdrop.h>
#include <node_view.h>
#include <knob_editor.h>
#include <tools.h>

trim_panel::trim_panel(properties *__properties, QString _name, QString _type,
                       QColor _color, QString _icon_name,
                       nodes_load *_nodes_loaded, project_struct *_project,
                       QWidget *__vinacomp, QGraphicsItem *_node,
                       QWidget *__node_view)

    : _knob_editor(nullptr)
    , this_node(_node)
    , nodes_loaded(_nodes_loaded)
    , _vinacomp(__vinacomp)
    , float_dock(0)
    , panel_is_float(false)
    , project(_project)
    , _node_view(__node_view)

    , is_maximize(true)
    , _properties(__properties)
    , _node_gui(nullptr)
    , name(_name)
    , type(_type)
    , icon_name(_icon_name)
    , color(_color)
    , params(project->nodes[_name].params)
    , custom_knobs(project->nodes[_name].custom_knobs)

{
    _knob_editor = static_cast<knob_editor *>(_properties->get_knob_editor());

    knobs = new QMap<QString, knob *>;
    curve_menu = new knob_curve_menu(this);

    this->setObjectName("trim_panel");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setSpacing(0);
    layout->setMargin(0);

    buttons = setup_tool_bar();
    layout->addWidget(buttons);

    // obtiene la lista de viewers en una lista de viewers pero con 'QWidget'
    // para usarlos con static_cast y no tener que importar el viewer.h a cada
    // knob
    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);
    viewers_gl = vina->get_viewers_gl();

    shared_knobs = jread("source/engine/nodes/json/shared_params.json")
                       .value("knobs")
                       .toArray();

    base_knobs = nodes_loaded->get_effect(type).value("knobs").toArray();
    QStringList finded_tabs = get_tabs_from_knobs(base_knobs);

    tabs = new tab_widget(false, _knob_editor, this);
    connect(tabs, &tab_widget::closed_tab, this,
            [=](QString tab_name, QWidget *widget) {
                delete_tab(tab_name);
                delete widget;
            });
    layout->addWidget(tabs);

    // añade cada tabs, 'controls' y 'node' son por defecto
    tabs_only_read = finded_tabs;
    tabs_only_read.push_back("Controls");
    tabs_only_read.push_back("Node");

    add_tab("Controls");
    for (QString tab_name : finded_tabs)
        add_tab(tab_name);
    add_tab("Node");
    //

    update_custom_knobs();

    tabs->set_index(0);
    //

    // lineas divisoras para el editor de 'knobs'
    dividing_line_v = new QWidget;
    dividing_line_h = new QWidget;

    dividing_line_h->hide();
    dividing_line_h->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dividing_line_h->setMinimumHeight(3);
    dividing_line_h->setStyleSheet(
        "border-bottom: 2px solid green; border-style: dotted;");

    dividing_line_v->hide();
    dividing_line_v->setMinimumWidth(3);
    dividing_line_v->setStyleSheet("border-left: 2px solid green;");
}

trim_panel::~trim_panel()
{
    leave_properties(false);

    delete dividing_line_h;
    delete dividing_line_v;

    delete layout;

    if (_node_gui)
        delete _node_gui;

    if (float_dock)
        delete float_dock;

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

QStringList trim_panel::get_tabs_from_knobs(QJsonArray _knobs)
{
    // encuentra todos los tab que estan en los '_knobs'
    QStringList finded_tabs;
    for (QJsonValue value : _knobs)
    {
        QString tab_name = value.toObject().value("tab").toString();
        if (!tab_name.isEmpty() && !finded_tabs.contains(tab_name))
            finded_tabs.push_back(tab_name);
    }

    return finded_tabs;
}

void trim_panel::update_custom_knobs()
{
    QStringList custom_knobs_names;
    for (QJsonValue value : *custom_knobs)
        custom_knobs_names.push_back(value.toObject().value("name").toString());

    // borra todos los 'knobs' custom solo los 'knobs' y no el 'custom_knobs'
    // json ni de los 'params' del proyecto como lo hace la funcion
    // 'remove_custom_knob'
    for (QString knob_name : custom_knobs_names)
    {
        knob *_knob = knobs->value(knob_name);
        if (!_knob)
            continue;

        delete _knob;
        knobs->remove(knob_name);
    }
    //

    clean_empty_line_widget();

    for (QString tab_name : get_tabs_from_knobs(*custom_knobs))
        add_tab(tab_name);
}

void trim_panel::clean_empty_line_widget()
{
    // borra todos los 'line_widget' vacios que queden al eliminar algun 'knob'
    // 'line_widget' se genera al crear un 'knob' sobre otro 'over_line'

    for (tab *_tab : tabs->get_tabs())
    {
        if (tabs_only_read.contains(_tab->get_name()))
            continue;

        QList<QWidget *> childers =
            _tab->get_widget()->findChildren<QWidget *>("line_widget");

        for (QWidget *line_widget : childers)
            if (line_widget->children().count() <= 1)
                // el 1 que hay es el layout
                delete line_widget;
    }
}

void trim_panel::set_edit_mode(bool enable)
{
    for (tab *_tab : tabs->get_tabs())
    {
        if (!tabs_only_read.contains(_tab->get_name()))
        {
            _tab->set_visible_edit_button(enable);
            _tab->set_visible_close_button(enable);
        }
    }

    for (QJsonValue value : *custom_knobs)
    {
        QString knob_name = value.toObject().value("name").toString();
        knob *_knob = knobs->value(knob_name);
        if (_knob)
            _knob->set_edit_mode(enable);
    }

    if (!enable)
    {
        dividing_line_h->hide();
        dividing_line_v->hide();

        dividing_line_h->setParent(0);
        dividing_line_v->setParent(0);
    }
}

void trim_panel::setup_shared_params()
{
    // crea las conecciones a los parametros que estan en el tab de 'node'
    node *_this_node = static_cast<node *>(this_node);

    // Etiquta Nodo
    knob_text *_knob_text = static_cast<knob_text *>(get_knob("label"));
    connect(_knob_text, &knob_text::text_changed, this, [=](QString text) {
        _this_node->set_tips(text);
        _this_node->refresh();
    });

    // Desabilitacion de Nodo
    knob_check_box *disable_node_knob =
        static_cast<knob_check_box *>(get_knob("disable_node"));
    connect(disable_node_knob, &knob_check_box::changed, this,
            [=](bool changed) {
                static_cast<node_rect *>(_this_node)->set_disable(changed);
            });

    // Color Nodo
    knob_color *node_color_knob =
        static_cast<knob_color *>(get_knob("node_color"));
    node_color_knob->disable_alpha();
    QColor color = _this_node->get_color();

    node_color_knob->set_init_color((float)color.red() / 255,
                                    (float)color.green() / 255,
                                    (float)color.blue() / 255);

    connect(node_color_knob, &knob_color::changed, this,
            [=](float red, float green, float blue) {
                _this_node->set_color(
                    {int(red * 255), int(green * 255), int(blue * 255)});
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
    else if (type == "group")
        _node_gui = new group_gui(nodes_loaded);
    else if (type == "roto")
        _node_gui = new roto_gui(_layout);
    else if (type == "shuffle")
    {
        knob_data = _knobs[0].toObject();
        _node_gui = new shuffle_gui(_layout);
    }

    if (_node_gui)
    {
        _node_gui->setup(this, _vinacomp, params, knob_data, name);
        _node_gui->restore_param();
    }
}

QWidget *trim_panel::setup_tool_bar()
{
    int icon_size = 20;

    QPushButton *icon_node = new QPushButton();
    if (icon_name.contains("/"))
        qt::set_icon(icon_node, icon_name, icon_size);
    else
        qt::set_icon(icon_node, icon_name + "_normal", icon_size);

    node *_node = static_cast<node *>(this_node);

    name_edit = new QLineEdit();
    connect(name_edit, &QLineEdit::editingFinished, [=]() {
        if (name_edit->text() == label)
            return;

        bool renamed = static_cast<node_view *>(_node_view)
                           ->rename_node(_node, name_edit->text());

        if (!renamed)
            name_edit->setText(label);
    });

    set_name(name);
    set_label_by_name(name);

    //
    tools *tool_bar = new tools(icon_size);

    // Acciones
    action *center_node_action = new action("Center Node", "", "center");
    action *restore_action = new action("Restore default values", "", "restart");
    minimize_action = new action("Minimize Panel", "", "minimize");
    action *float_panel_action = new action("Float Panel", "", "float_panel");
    action *close_action = new action("Close Panel", "", "close");

    // Conecciones
    center_node_action->connect_to(this, [=]() {
        static_cast<node_view *>(_node_view)
            ->center_node(static_cast<node *>(this_node));
    });

    restore_action->connect_to(this, [this]() { restore_default_values(); });

    minimize_action->connect_to(this, [=]() { this->maximize(!is_maximize); });

    float_panel_action->connect_to(this,
                                   [this]() { float_panel(!panel_is_float); });

    close_action->connect_to(this, [this]() {
        _properties->close_trim_panel(this->get_name());
        float_panel(false, false);
    });

    // Layout
    tool_bar->add_action(center_node_action);
    tool_bar->add_action(restore_action);

    tool_bar->add_stretch();

    tool_bar->add_widget(icon_node);
    tool_bar->add_widget(name_edit);

    tool_bar->add_stretch();

    tool_bar->add_action(minimize_action);
    tool_bar->add_action(float_panel_action);
    tool_bar->add_action(close_action);


    return tool_bar;
}

void trim_panel::add_tab(QString tab_name, int index)
{
    // si el tab no existe lo crea
    tab *_tab = tabs->get_tab(tab_name);
    QVBoxLayout *tab_layout;
    if (!_tab)
    {
        QWidget *widget = new QWidget(this);
        tab_layout = new QVBoxLayout(widget);
        tab_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        tab_layout->setSpacing(5);
        widget->setObjectName("knobs_tab");

        tabs->add_tab(widget, tab_name, index);
    }
    else
    {
        tab_layout = static_cast<QVBoxLayout *>(_tab->get_widget()->layout());
    }
    //

    // custom tabs
    if (!tabs_only_read.contains(tab_name))
    {
        QJsonArray this_tab_custom_knobs;
        for (QJsonValue knob : *custom_knobs)
        {
            QString _tab_name = knob.toObject().value("tab").toString();
            if (_tab_name == tab_name)
                this_tab_custom_knobs.push_back(knob);
        }

        setup_knobs(this_tab_custom_knobs, tab_layout, viewers_gl);
        return;
    }
    //

    if (tab_name == "Node")
    {
        setup_knobs(shared_knobs, tab_layout, viewers_gl);
        setup_shared_params();

        return;
    }

    QJsonArray this_tab_knobs;
    for (QJsonValue knob : base_knobs)
    {
        QString _tab_name = knob.toObject().value("tab").toString();

        // si el tab del knob esta vacio, queda en el tab por defecto que es 'controls'
        if (tab_name == "Controls" && _tab_name.isEmpty())
        {
            this_tab_knobs.push_back(knob);
            continue;
        }

        if (_tab_name == tab_name)
            this_tab_knobs.push_back(knob);
    }

    setup_knobs(this_tab_knobs, tab_layout, viewers_gl);

    if (tab_name == "Controls")
        setup_gui_panels(this_tab_knobs, tab_layout);
}

void trim_panel::delete_tab(QString tab_name)
{
    // no usar esta funcion, solo usar en retorno de señal del closed del
    // tab_widget, por que el puntero de del widget no de borra, si se quere
    // usar esta funcion borrar manualmente el widget 'delete' despues de
    // invocar esta funcion
    QStringList knob_to_delete;
    for (QJsonValue knob : *custom_knobs)
    {
        QString tab_knob = knob.toObject().value("tab").toString();
        if (tab_knob == tab_name)
            knob_to_delete.push_back(knob.toObject().value("name").toString());
    }

    for (QString knob_name : knob_to_delete)
        remove_custom_knob(knob_name);
}

void trim_panel::remove_custom_knob(QString knob_name)
{
    QJsonArray new_custom_knobs;
    for (QJsonValue knob : *custom_knobs)
    {
        QString _knob_name = knob.toObject().value("name").toString();
        if (_knob_name != knob_name)
            new_custom_knobs.push_back(knob);
    }

    *custom_knobs = new_custom_knobs;

    knob *_knob = knobs->value(knob_name);
    if (_knob)
        delete _knob;

    params->remove(knob_name);
    params->remove(knob_name + "_curve");
    params->remove(knob_name + "_exp");

    knobs->remove(knob_name);

    clean_empty_line_widget();
}

void trim_panel::maximize(bool _maximize)
{
    tabs->setVisible(_maximize);
    is_maximize = _maximize;

    if (is_maximize)
    {
        minimize_action->set_icon("minimize");
        minimize_action->set_tool_tip("Minimize Panel");
    }
    else
    {
        minimize_action->set_icon("maximize");
        minimize_action->set_tool_tip("Maximize Panel");
    }

    for (QWidget *vgl : *viewers_gl)
        static_cast<viewer_gl *>(vgl)->handlers_update();
}

void trim_panel::update_render()
{
    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer();
}

void trim_panel::enter_to_properties()
{
    minimize_action->set_visible(true);
    if (float_dock)
    {
        float_dock->hide();
        panel_is_float = false;
    }

    this->maximize(true);
    this->show();
    set_edit_mode(_properties->is_edit_mode());
}

void trim_panel::restore_default_values()
{
    for (knob *_knob : *knobs)
        _knob->restore_default();
}

void trim_panel::float_panel(bool enable, bool relocate)
{
    if (!float_dock)
    {
        float_dock = new QDockWidget(_vinacomp);
        float_dock->setTitleBarWidget(new QWidget);
        float_dock->setObjectName("trim_panel_float_dock");
        float_dock->setFixedWidth(500);
    }

    if (!enable)
        float_dock->hide();

    panel_is_float = enable;

    minimize_action->set_visible(!enable);
    maximize(true);

    float_dock->setWidget(this);
    float_dock->setFloating(true);

    float_dock->move(QCursor::pos().x() - 250, QCursor::pos().y() - 50);

    if (enable)
        float_dock->show();

    if (!enable && relocate)
        _properties->add_trim_panel(this);
}

void trim_panel::leave_properties(bool disable_edit_mode)
{
    if (disable_edit_mode)
        this->set_edit_mode(false);

    this->hide();
    this->setParent(0);

    // a veces se queda pegado al presionar otros click cuando se esta
    // arrastrando el knob en el 'knob_editor', para evitar conflictos, finaliza
    // la insercion si se quedan pegadas las lineas divisoras, ya que en el
    // 'knob_editor' se estan usando 'dividing_line_h' y 'dividing_line_v',
    // tambien termina el modo de edicion de 'knobs'
    _knob_editor->finish_insertion(false);
    _knob_editor->finish_edit(false);
    //
}

void trim_panel::unlink_all()
{
    for (knob *_knob : *knobs)
        _knob->remove_link();
}

void trim_panel::mousePressEvent(QMouseEvent *event)
{
    // a veces se queda pegado al presionar otros click cuando se esta
    // arrastrando el knob en el 'knob_editor', para evitar conflictos, finaliza
    // la insercion si se quedan pegadas las lineas divisoras, ya que en el
    // 'knob_editor' se estan usando 'dividing_line_h' y 'dividing_line_v'
    _knob_editor->finish_insertion(false);
    //
}

void trim_panel::mouseMoveEvent(QMouseEvent *event)
{
    if (_properties->is_edit_mode())
        _knob_editor->drag_knob_move(this);
}

void trim_panel::mouseReleaseEvent(QMouseEvent *event)
{
    if (_properties->is_edit_mode())
        _knob_editor->drag_knob_release(this);
}
