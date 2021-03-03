#include <panel.h>
#include <vinacomp.h>
#include <panels_layout.h>

panel::panel(
		QWidget *__panels_layout,
		QWidget *__vinacomp,
		QList<QSplitter *> *_splitters,
		node_graph *__node_graph,
		QLabel *_empty_viewer,
		script_editor *__script_editor,
		properties *__properties,
		curve_editor *__curve_editor)

    : _panels_layout(__panels_layout)
	, _vinacomp(__vinacomp)
	, _node_graph(__node_graph)
	, empty_viewer(_empty_viewer)
	, _script_editor(__script_editor)
	, _properties(__properties)
	, _curve_editor(__curve_editor)
	, splitters(_splitters)

{
    this->setObjectName("panel");

    _tab_widget = new tab_widget(true);
	connect(_tab_widget, &tab_widget::closed_tab, this,[=](QString tab_name){
        tabs_list.removeOne(tab_name);
	});
    QPushButton *cornel_button = setup_cornel_buttons();

    QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0);
    layout->addWidget(_tab_widget);

}

panel::~panel()
{
}

QPushButton *panel::setup_cornel_buttons()
{
    QPushButton *menu_button = _tab_widget->add_cornel_button("layout_a");

    // Menu
    QAction *split_vertical = new QAction("Vertical Split");
    connect(split_vertical, &QAction::triggered, this, [this]() {
        split(Qt::Horizontal);
    });
    split_vertical->setIcon(QIcon("resources/images/vertical_split_a.png"));

    QAction *split_horizontal = new QAction("Horizontal Split");
    connect(split_horizontal, &QAction::triggered, this, [this]() {
        split(Qt::Vertical);
    });
    split_horizontal->setIcon(QIcon("resources/images/horizontal_split_a.png"));

    // Cerrar panel
    QAction *close_panel_action = new QAction("Close Panel");
    connect(close_panel_action, &QAction::triggered, this, [this]() {
        close_panel();
    });
    close_panel_action->setIcon(QIcon("resources/images/close_a.png"));
    //

    // Add NodeGraph
    QAction *add_node_graph_action = new QAction("Node Graph");
    connect(add_node_graph_action, &QAction::triggered, this, [this]() {
        add_fixed_panel("node_graph");
    });
    add_node_graph_action->setIcon(QIcon("resources/images/node_graph_a.png"));
    //

    // Add Script Editor
    QAction *add_script_editor_action = new QAction("Script Editor");
    connect(add_script_editor_action, &QAction::triggered, this, [this]() {
        add_fixed_panel("script_editor");
    });
    add_script_editor_action->setIcon(QIcon("resources/images/script_editor_a.png"));
    //

    // Add Curve Editor
    QAction *add_curve_editor_action = new QAction("Curve Editor");
    connect(add_curve_editor_action, &QAction::triggered, this, [this]() {
        add_fixed_panel("curve_editor");
    });
    add_curve_editor_action->setIcon(QIcon("resources/images/curve_editor_a.png"));
    //

    // Add Properties
    QAction *add_properties_action = new QAction("Properties");
    connect(add_properties_action, &QAction::triggered, this, [this]() {
        add_fixed_panel("properties");
    });
    add_properties_action->setIcon(QIcon("resources/images/properties_a.png"));
    //

    // Add Viewer
    QAction *add_viewer_action = new QAction("Viewer");
    connect(add_viewer_action, &QAction::triggered, this, [this]() {
		// si no existe ningun viewer creado, abre el 'empty_viewer'
		auto *viewers = get_viewers();
		if (viewers->empty())
			add_fixed_panel("viewer");
		else
			add_viewer(viewers->value(0));
    });
    add_viewer_action->setIcon(QIcon("resources/images/viewer_a.png"));
    //

    // New Viewer
    QAction *new_viewer_action = new QAction("New Viewer");
    connect(new_viewer_action, &QAction::triggered, this, [this]() {
		_node_graph->get_maker()->create_fx("viewer");
    });
    new_viewer_action->setIcon(QIcon("resources/images/viewer_a.png"));
    //

	// Viewers Menu
	viewers_menu = new QMenu("Viewers", this);
	update_viewers_menu();
	//

    QMenu *menu = new QMenu(this);

    menu->addAction(split_vertical);
    menu->addAction(split_horizontal);
    menu->addSeparator();
    menu->addAction(add_node_graph_action);
    menu->addAction(add_curve_editor_action);
    menu->addAction(add_script_editor_action);
    menu->addAction(add_properties_action);
    menu->addSeparator();
    menu->addAction(add_viewer_action);
    menu->addAction(new_viewer_action);
	menu->addMenu(viewers_menu);
    menu->addSeparator();
    menu->addAction(close_panel_action);
    //
    //

    connect(menu_button, &QPushButton::clicked, this, [=]() {
        menu->exec(menu_button->mapToGlobal({0, menu_button->height()}));
    });

    return menu_button;
}

void panel::add_tabs(QStringList tabs_list)
{
    for (QString tab_name : tabs_list)
        add_fixed_panel(tab_name);
}

void panel::remove_all_tab()
{
    _tab_widget->clear();
    tabs_list.clear();
}

QString panel::get_tab_label(QString name)
{
    if (name == "node_graph")
        return "Node Graph";

    else if (name == "viewer")
        return "Viewer";

    else if (name == "curve_editor")
        return "Curve Editor";

    else if (name == "script_editor")
        return "Script Editor";

    else if (name == "properties")
        return "Properties";

    else
        return "";
}

void panel::set_index(int index)
{
    _tab_widget->set_index(index);
}

void panel::add_fixed_panel(QString name)
{
	// añade un tab de algun panel fijo
    QWidget *tab;
    if (name == "node_graph")
        tab = _node_graph;

    else if (name == "viewer")
        tab = empty_viewer;

    else if (name == "curve_editor")
        tab = _curve_editor;

    else if (name == "script_editor")
        tab = _script_editor;

    else if (name == "properties")
        tab = _properties;

    else
        return;

	add_tab(tab, name);
}

void panel::add_tab(QWidget *widget, QString name)
{
	QString label = get_tab_label(name);
	if (label.isEmpty())
		label = name;

    _tab_widget->add_tab(widget, label);

    // el tab que se va a agregar en este panel se borra en
    // todos los paneles, si es que esta en alguno.
	auto panels = static_cast<panels_layout*>(_panels_layout)->get_all_panels();

    for (panel *_panel : panels)
        _panel->tabs_list.removeOne(label);
    //
    //

    tabs_list.push_back(label);
}

void panel::add_viewer(viewer *_viewer)
{
	_tab_widget->remove_tab("Viewer");
	tabs_list.removeOne("viewer");
	add_tab(_viewer, _viewer->get_name());

	// actualiza el munu de visores de todos los paneles
	auto panels = static_cast<panels_layout*>(_panels_layout)->get_all_panels();
    for (panel *_panel : panels)
		_panel->update_viewers_menu();
	//
}

void panel::update_viewers_menu()
{
	viewers_menu->clear();
	for (viewer *_viewer : *get_viewers())
	{
		QAction *viewer_action = new QAction(_viewer->get_name());
		connect(viewer_action, &QAction::triggered, this, [=]() {
			add_viewer(_viewer);
		});
		viewer_action->setIcon(QIcon("resources/images/viewer_a.png"));
		viewers_menu->addAction(viewer_action);
	}
}

QList <viewer*> *panel::get_viewers() const
{
	return static_cast<vinacomp *>(_vinacomp)->get_viewers();
}

QSplitter *panel::get_splitter() const
{
    QWidget *container = this->parentWidget();
    QWidget *qsplitter = container->parentWidget();

    QSplitter *splitter;
    for (QSplitter *_splitter : *splitters)
    {
        if (_splitter->objectName() == qsplitter->objectName())
        {
            splitter = _splitter;
            break;
        }
    }

    return splitter;
}

QStringList panel::get_tabs_list() const
{
	return tabs_list;
}

tab_widget *panel::get_tab_widget() const
{
	return _tab_widget;
}

panel *panel::split(Qt::Orientation orientation)
{

    auto parent = this->parentWidget();
    QLayout *layout = parent->layout();

    QSplitter *qsplitter = new QSplitter();

    if (parent->objectName() == "panels_layout")
        qsplitter->setObjectName("splitter");
    else
        qsplitter->setObjectName("splitter" + util::hash());

    splitters->push_back(qsplitter);

    qsplitter->setOrientation(orientation);

    panel *new_panel = new panel(_panels_layout, _vinacomp, splitters, _node_graph, empty_viewer, _script_editor, _properties, _curve_editor);

    QWidget *container_a = new QWidget();
    QWidget *container_b = new QWidget();
    container_a->setObjectName("container_a");
    container_b->setObjectName("container_b");

    qt::add_widget(container_a, this);
    qt::add_widget(container_b, new_panel);

    container_a->setParent(qsplitter);
    container_b->setParent(qsplitter);

    qsplitter->addWidget(container_a);
    qsplitter->addWidget(container_b);

    qsplitter->setSizes({100, 100});

    layout->addWidget(qsplitter);

    return new_panel;
}

void panel::close_panel()
{
    QWidget *container = this->parentWidget();

    // si el container es el 'panels_layout' significa que es el
    // ultimo widget, y no se puede eliminar.
    if (container->objectName() == "panels_layout")
        return;
    //

    remove_all_tab();

    QWidget *qsplitter = container->parentWidget();
    QWidget *parent = qsplitter->parentWidget();

    QWidget *keep_widget;
    QWidget *delete_widget;

    if (container->objectName() == "container_a")
    {
        keep_widget = qsplitter->findChild<QWidget *>("container_b");
        delete_widget = qsplitter->findChild<QWidget *>("container_a");
    }
    else
    {
        keep_widget = qsplitter->findChild<QWidget *>("container_a");
        delete_widget = qsplitter->findChild<QWidget *>("container_b");
    }

    panel *keep_panel = keep_widget->findChild<panel *>();
    parent->layout()->addWidget(keep_panel);

    // borra los widgets sin usar
    delete_widget->setParent(0);
    delete_widget->deleteLater();

    // Borra el 'qsplitter' de la lista de 'splitters'
    int i = 0;
    for (QSplitter *splitter : *splitters)
    {
        if (splitter->objectName() == qsplitter->objectName())
            break;
        i++;
    }
    splitters->removeAt(i);
    //
    //

    delete _tab_widget;

    parent->layout()->removeWidget(qsplitter);
    qsplitter->setParent(0);
    qsplitter->deleteLater();
}
