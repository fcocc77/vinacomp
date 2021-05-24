#include <panels_layout.h>
#include <vinacomp.h>
#include <global.h>

panels_layout::panels_layout(QWidget *__vinacomp, node_graph *_node_graph, QLabel *empty_viewer,
                             script_editor *_script_editor, properties *_properties,
                             curve_editor *_curve_editor)

    : isolate(false)
    , _vinacomp(__vinacomp)
{

    this->setObjectName("panels_layout");

    splitters = new QList<QSplitter *>;

    first_panel = new panel(this, _vinacomp, splitters, _node_graph, empty_viewer, _script_editor,
                            _properties, _curve_editor);

    qt::add_widget(this, first_panel);

    restore_default_action =
        new action("Restore Default Layout", "", "restart");
    save_current_action = new action("Save Current Layout", "", "save");
    script_layout_action = new action("Script Layout", "", "code");
    comp_layout_action = new action("Comp Layout", "", "image");
    all_visible_layout_action = new action("All Visible Layout", "", "layout");

    connect(save_current_action, &QAction::triggered, this,
            &panels_layout::save_layout);
    connect(restore_default_action, &QAction::triggered, this,
            &panels_layout::restore_default);
    script_layout_action->connect_to(this,
                                     [this]() { set_layout("script_layout"); });
    comp_layout_action->connect_to(this,
                                   [this]() { set_layout("comp_layout"); });
    all_visible_layout_action->connect_to(
        this, [this]() { set_layout("all_layout"); });

    load_layout();

    qt::shortcut("Space", this, [this]() { isolate_panel(); });

    for (int i = 0; i < 9; i++)
    {
        qt::shortcut("Alt+" + QString::number(i + 1), this, [=]() {
            panel *_panel = get_panel_from_cursor();
            if (_panel)
                _panel->set_index(i);
        });
    }
}

panels_layout::~panels_layout() {}

panel *panels_layout::get_panel_from_cursor() const
{
    // Encuentra el panel a partir del cursor
    QWidget *_panel = qApp->widgetAt(QCursor::pos());
    while (_panel)
    {
        QString name = _panel->objectName();
        if (name == "panel")
            break;

        _panel = _panel->parentWidget();
    }

    return static_cast<panel *>(_panel);
}

panel *panels_layout::get_child_panel(QSplitter *splitter, QString _letter) const
{
    // Obtiene el widget hijo del splitter 'a' o 'b'
    QWidget *container;
    if (_letter == "a")
        container = splitter->widget(0);
    else
        container = splitter->widget(1);

    return container->findChild<panel *>();
}

QList<panel *> panels_layout::get_all_panels() const
{
    return this->findChildren<panel *>("panel");
}

void panels_layout::add_viewer(viewer *_viewer)
{
    auto panels = get_all_panels();

    // si el viewer ya esta en algun panel lo visualiza y retorna
    for (panel *_panel : panels)
    {
        QString name = _viewer->get_name();
        if (_panel->get_tabs_list().contains(name))
        {
            _panel->get_tab_widget()->set_tab(name);
            return;
        }
    }

    panel *empty_panel = get_some_empty_panel();
    panel *viewer_panel = get_panel("Viewer");

    // panel a insertar el nuevo 'viewer'
    // primero busca donde hay algun panel con algun 'viewer' si no, busca un
    // panel vacio, si no lo inserte en el primer panel de la lista
    if (viewer_panel)
        viewer_panel->add_viewer(_viewer);
    else if (empty_panel)
        empty_panel->add_viewer(_viewer);
    else
        panels.value(0)->add_viewer(_viewer);
}

void panels_layout::delete_viewer(viewer *_viewer)
{
    panel *viewer_panel = get_panel(_viewer->get_name());
    if (!viewer_panel)
    {
        for (panel *_panel : get_all_panels())
            _panel->update_all_viewers_menu();

        return;
    }

    viewer_panel->remove_viewer(_viewer);
}

panel *panels_layout::get_panel(QString name) const
{
    // encuentra el panel por nombre
    for (panel *_panel : get_all_panels())
        for (QString tab_name : _panel->get_tabs_list())
            if (tab_name.contains(name))
                return _panel;

    return nullptr;
}

void panels_layout::add_node_graph_group(node_graph *group, QString name)
{
    auto groups = static_cast<vinacomp*>(_vinacomp)->get_groups_node_graph();

    // encuentra el panel que contenga algun grupo
    panel *group_panel = nullptr;
    for (QString group_graph : groups->keys())
    {
        group_panel = get_panel(group_graph);
        if (group_panel)
            break;
    }
    //

    panel *node_graph_panel = get_panel("Node Graph");

    // inserta el grupo en el panel donde contenga un grupo, sino donde este el
    // node_graph principal y si no lo inserta en el primer panel de la lista

    panel *_panel;
    if (group_panel)
        _panel = group_panel;
    else if (node_graph_panel)
        _panel = node_graph_panel;
    else
        _panel = get_all_panels().value(0);

    _panel->add_group(group);
}

void panels_layout::delete_node_graph_group(node_graph *group)
{
    panel *group_panel = get_panel(group->get_group_name());
    if (!group_panel)
    {
        for (panel *_panel : get_all_panels())
            _panel->update_all_group_menu();
        return;
    }

    group_panel->remove_group(group);
}

void panels_layout::rename_node_graph_group(QString old_name, QString new_name)
{
    panel *group_panel = get_panel(old_name);
    if (!group_panel)
        return;

    group_panel->rename_group(old_name, new_name);
}

panel *panels_layout::get_some_empty_panel() const
{
    // obtiene el primer panel vacio que encuentra
    for (panel *_panel : get_all_panels())
        if (_panel->get_tabs_list().empty())
            return _panel;

    return nullptr;
}

void panels_layout::open_script_editor()
{
    panel *script_panel = get_panel("Script Editor");
    if (script_panel)
    {
        script_panel->get_tab_widget()->set_tab("Script Editor");
        return;
    }

    script_panel = get_all_panels().first();
    if (!script_panel)
        return;

    script_panel->add_fixed_panel("Script Editor");
}

void panels_layout::isolate_panel()
{
    set_visible_panels(true);

    // encuentra el container del panel
    QWidget *container = qApp->widgetAt(QCursor::pos());
    while (container != NULL)
    {
        QString name = container->objectName();
        if (name == "container_a" || name == "container_b")
            break;

        container = container->parentWidget();
    }

    if (container == NULL)
        return;

    isolate = !isolate;

    if (isolate)
    {
        set_visible_panels(false);
        QWidget *parent = container;
        while (parent)
        {
            parent->setVisible(true);
            parent = parent->parentWidget();
        }
    }
}

void panels_layout::set_visible_panels(bool visible)
{
    // ajusta visibilidad de todos los container a y b
    QList<QWidget *> containers_a = this->findChildren<QWidget *>("container_a");
    for (QWidget *container : containers_a)
        container->setVisible(visible);

    QList<QWidget *> containers_b = this->findChildren<QWidget *>("container_b");
    for (QWidget *container : containers_b)
        container->setVisible(visible);
    //
}

void panels_layout::save_json_layout(QSplitter *splitter, int deep, QString letter,
                                     QStringList parents)
{
    if (!splitter)
        return;

    if (!letter.isEmpty())
    {
        panel *child_panel = get_child_panel(splitter, letter);

        if (child_panel->objectName() == "panel")
            return;
        else
        {
            for (QSplitter *_splitter : *splitters)
            {
                if (_splitter->objectName() == child_panel->objectName())
                {
                    splitter = _splitter;
                    break;
                }
            }
        }
    }

    deep++;

    QString parent_name;
    if (deep == 1)
        parent_name = "splitter";
    else
        parent_name = "splitter_" + letter;
    parents.push_back(parent_name);

    int orientation = splitter->orientation();
    auto sizes = splitter->sizes();
    //
    //

    QJsonObject value;

    value["orientation"] = orientation;
    value["distribution"] = QJsonArray{sizes[0], sizes[1]};

    // Crea los objetos a y b de un splitter y si el hijo es un panel,
    // guarda los datos de los 'tabs' del panel.
    panel *child_panel_a = get_child_panel(splitter, "a");
    panel *child_panel_b = get_child_panel(splitter, "b");

    QJsonObject splitter_a;
    QJsonObject splitter_b;
    if (child_panel_a->objectName() == "panel")
    {
        QJsonObject panel_a_data;
        panel_a_data["tabs"] = qt::list_to_array(child_panel_a->get_tabs_list());
        panel_a_data["current_index"] = child_panel_a->get_tab_widget()->get_current_index();
        value["panel_a"] = panel_a_data;
    }
    else
        value["splitter_a"] = {};

    if (child_panel_b->objectName() == "panel")
    {
        QJsonObject panel_b_data;
        panel_b_data["tabs"] = qt::list_to_array(child_panel_b->get_tabs_list());
        panel_b_data["current_index"] = child_panel_b->get_tab_widget()->get_current_index();
        value["panel_b"] = panel_b_data;
    }
    else
        value["splitter_b"] = {};
    //
    //

    qt::insert_json_deep(&json_layout, parents, value);

    save_json_layout(splitter, deep, "a", parents);
    save_json_layout(splitter, deep, "b", parents);
}

void panels_layout::save_layout()
{
    QSplitter *main_splitter = this->findChild<QSplitter *>();

    // vaciar layout json
    json_layout = {};
    //

    save_json_layout(main_splitter);
    jwrite(VINACOMP_CONF_PATH + "/layout.json", json_layout);
}

void panels_layout::load_splitter(QJsonObject splitter_obj, panel *panel_a)
{
    QJsonArray distribution = splitter_obj["distribution"].toArray();
    int distribution_a = distribution[0].toInt();
    int distribution_b = distribution[1].toInt();

    int orientation = splitter_obj["orientation"].toInt();

    panel *panel_b = panel_a->split(Qt::Orientation(orientation));
    panel_b->get_splitter()->setSizes({distribution_a, distribution_b});

    QJsonObject splitter_a = splitter_obj["splitter_a"].toObject();
    QJsonObject splitter_b = splitter_obj["splitter_b"].toObject();

    QJsonObject panel_a_obj = splitter_obj["panel_a"].toObject();
    QJsonObject panel_b_obj = splitter_obj["panel_b"].toObject();

    if (!splitter_a.isEmpty())
        load_splitter(splitter_a, panel_a);
    if (!splitter_b.isEmpty())
        load_splitter(splitter_b, panel_b);

    if (!panel_a_obj.isEmpty())
    {
        panel_a->add_tabs(qt::array_to_list(panel_a_obj["tabs"].toArray()));
        panel_a->get_tab_widget()->set_index(panel_a_obj["current_index"].toInt());
    }
    if (!panel_b_obj.isEmpty())
    {
        panel_b->add_tabs(qt::array_to_list(panel_b_obj["tabs"].toArray()));
        panel_b->get_tab_widget()->set_index(panel_b_obj["current_index"].toInt());
    }
}

void panels_layout::load_layout()
{
    QString custom_layout = VINACOMP_CONF_PATH + "/layout.json";
    QJsonObject layout;

    if (os::isfile(custom_layout))
        layout = jread(custom_layout);
    else
        layout = jread("resources/data/default_layout.json");

    QJsonObject main = layout["splitter"].toObject();
    load_splitter(main, first_panel);
}

void panels_layout::restore_default()
{
    set_layout("default_layout");
}

void panels_layout::set_layout(QString layout_name)
{
    QString layout_path = "resources/data/" + layout_name + ".json";
    if (!os::isfile(layout_path))
        return;

    for (panel *_panel : get_all_panels())
    {
        _panel->remove_all_tab();
        _panel->close_panel();
    }

    QJsonObject layout = jread(layout_path);
    QJsonObject main = layout["splitter"].toObject();

    // encuentra el unico papel que queda después de eliminarlos todos,
    // y a partir de este panel se construyen todos los demas.
    panel *main_panel = this->findChild<panel *>("panel");
    //

    load_splitter(main, main_panel);
}
