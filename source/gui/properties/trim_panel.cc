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
#include <knob_editor.h>

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
    , params(project->nodes[_name].params)
    , custom_knobs(project->nodes[_name].custom_knobs)

{
    _knob_editor = static_cast<knob_editor *>(_properties->get_knob_editor());

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

    shared_knobs = jread("source/engine/nodes/json/shared_params.json")
                       .value("knobs")
                       .toArray();

    base_knobs = nodes_loaded->get_effect(type).value("knobs").toArray();
    QStringList finded_tabs = get_tabs_from_knobs(base_knobs);

    tabs = new tab_widget();
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
    leave_properties();

    delete dividing_line_h;
    delete dividing_line_v;

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
        if (!tabs_only_read.contains(_tab->get_name()))
            _tab->set_visible_close_button(enable);

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
        _node_gui->setup(this, _vinacomp, params, knob_data, name);
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
    qt::set_icon(center_node, "center_normal", icon_size);
    layout->addWidget(center_node);

    restart_button = new QPushButton(widget);
    connect(restart_button, &QPushButton::clicked, this, [=]() {  });
    qt::set_icon(restart_button, "restart_normal", icon_size);
    layout->addWidget(restart_button);

    layout->addStretch();

    QPushButton *icon_node = new QPushButton(widget);
    if (icon_name.contains("/"))
        qt::set_icon(icon_node, icon_name, icon_size);
    else
        qt::set_icon(icon_node, icon_name + "_normal", icon_size);
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
    qt::set_icon(minimize, "minimize_normal", icon_size);
    layout->addWidget(minimize);
    // Minimize
    //

    QPushButton *maximize_button = new QPushButton(widget);
    qt::set_icon(maximize_button, "maximize_normal", icon_size);
    layout->addWidget(maximize_button);

    QPushButton *close = new QPushButton(widget);
    connect(close, &QPushButton::clicked, this,
            [this]() { _properties->close_trim_panel(this->get_name()); });
    qt::set_icon(close, "close_normal", icon_size);
    layout->addWidget(close);

    return widget;
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

    if (tab_name == "Controls")
        setup_gui_panels(this_tab_knobs, tab_layout);

    setup_knobs(this_tab_knobs, tab_layout, viewers_gl);
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
    params->remove(knob_name + "_anim");
    params->remove(knob_name + "_exp");

    knobs->remove(knob_name);

    clean_empty_line_widget();
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

void trim_panel::enter_to_properties()
{
    this->maximize(true);
    this->show();
    set_edit_mode(_properties->is_edit_mode());
}

void trim_panel::leave_properties()
{
    this->set_edit_mode(false);
    this->hide();
    this->setParent(0);

    // a veces se queda pegado al presionar otros click cuando se esta
    // arrastrando el knob en el 'knob_editor', para evitar conflictos, finaliza
    // la insercion si se quedan pegadas las lineas divisoras, ya que en el
    // 'knob_editor' se estan usando 'dividing_line_h' y 'dividing_line_v',
    // tambien termina el modo de edicion de 'knobs'
    _knob_editor->finish_insertion(false);
    _knob_editor->finish_edit_knob(false);
    //
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
