#include <vinacomp.h>

vinacomp::vinacomp()
	: fullscreen(false)
	, project_settings_visible(false)
	, settings_visible(false)
	, project_opened(false)
	, current_project("")
{
    this->setObjectName("vinacomp");
    project_old = new QJsonObject();
	project = new project_struct();
    setup_ui();
}

vinacomp::~vinacomp()
{
}

void vinacomp::setup_ui()
{
	// Viewer Vacio
	QLabel *empty_viewer = new QLabel("( Viewer1 )");
	empty_viewer->setAlignment(Qt::AlignCenter);
	empty_viewer->setObjectName("empty_viewer");
	viewers = new QList <viewer*>;
	//

	//
    _properties = new properties();
    _node_graph = new node_graph(this, project_old, _properties);
    _script_editor = new script_editor(project_old, _node_graph);
    _curve_editor = new curve_editor();
    _settings = new settings();
	_project_settings = new project_settings(this);

    _panels_layout = new panels_layout(this, _node_graph, empty_viewer, _script_editor, _properties, _curve_editor);
    _tool_bar = new tools(25, true);

	// settings panel
	QWidget *settings_panel = new QWidget();
	settings_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QVBoxLayout *settings_panel_layout = new QVBoxLayout(settings_panel);
	settings_panel_layout->setMargin(0);
	settings_panel_layout->setSpacing(0);
	settings_panel_layout->addWidget(_settings);
	settings_panel_layout->addWidget(_project_settings);
	//

    QWidget *central_widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central_widget);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(_panels_layout);
    layout->addWidget(settings_panel);
    layout->addWidget(_tool_bar);

    this->setCentralWidget(central_widget);

    update_sylesheet_action = new QAction("Update StyleSheet");
    update_sylesheet_action->setShortcut(QString("Ctrl+R"));
    connect(update_sylesheet_action, &QAction::triggered, this, &vinacomp::setup_style);
    setup_style();

    main_menu();
	tool_bar();
}

panels_layout *vinacomp::get_panels_layout() const
{
	return _panels_layout;
}

QList <viewer*> *vinacomp::get_viewers() const
{
	return viewers;
}

project_struct *vinacomp::get_project() const
{
	return project;
}

void vinacomp::setup_style()
{
    QString style = fread("./resources/css/style.css");
    this->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana principal
    this->setWindowTitle("Untitled - VinaComp");
}

void vinacomp::main_menu()
{

    QMenuBar *menu_bar = new QMenuBar();

    QMenu *file_menu = new QMenu("File", menu_bar);
    menu_bar->addMenu(file_menu);

    new_project_action = new action("New Project", "Ctrl+N", "add");
    file_menu->addAction(new_project_action);

    open_project_action = new action("Open Project", "Ctrl+O", "folder");
    open_project_action->connect_to(this, [this]() { open_project_dialog(); });

    file_menu->addAction(open_project_action);

    recent_projects_menu = new QMenu("Recent Projects", this);
    update_recent_projects();
    file_menu->addMenu(recent_projects_menu);

    file_menu->addSeparator();

    action *save_project_action = new action("Save Project", "Ctrl+S", "save");
    save_project_action->connect_to(this, [this]() { to_save_project(); });
    file_menu->addAction(save_project_action);

    action *save_project_as = new action("Save Project As...", "Ctrl+Shift+S");
    save_project_as->connect_to(this, [this]() { save_as(); });
    file_menu->addAction(save_project_as);

    file_menu->addSeparator();

    action *quit = new action("Quit", "Ctrl+Q", "quit");
    quit->connect_to(this, [this]() { this->close(); });
    file_menu->addAction(quit);
    //
    //

    //
    QMenu *edit_menu = new QMenu("Edit", menu_bar);
    menu_bar->addMenu(edit_menu);

    settings_action = new action("Settings...", "Shift+S", "settings");
	settings_action->set_checkable(true);
    settings_action->connect_to(this, [this]() {
		settings_visible = !settings_visible; // Toggle
        _settings->setVisible(settings_visible);
    });

    project_settings_action = new action("Project Settings", "S", "project_settings");
	project_settings_action->set_checkable(true);
    project_settings_action->connect_to(this, [this]() {
		project_settings_visible = !project_settings_visible; // Toggle
        _project_settings->setVisible(project_settings_visible);
    });

    edit_menu->addAction(settings_action);
    edit_menu->addAction(project_settings_action);
    edit_menu->addAction(update_sylesheet_action);
    //
    //

    //
    QMenu *layout_menu = new QMenu("Layout", menu_bar);
    menu_bar->addMenu(layout_menu);

	script_layout_action = new action("Script Layout", "", "view_compact");
	layout_menu->addAction(script_layout_action);

	comp_layout_action = new action("Comp Layout", "", "vertical_split");
	layout_menu->addAction(comp_layout_action);

	layout_menu->addSeparator();

    layout_menu->addAction(_panels_layout->restore_default_action);
    _panels_layout->restore_default_action->setIcon(QIcon("resources/images/layout_a.png"));

    layout_menu->addAction(_panels_layout->save_current_action);
    _panels_layout->save_current_action->setIcon(QIcon("resources/images/save_a.png"));

    //
    //

    QMenu *display = new QMenu("Display", menu_bar);
    menu_bar->addMenu(display);

    action *full_screen = new action("Full Screen", "Alt+S", "fullscreen");
    full_screen->connect_to(this, [this]() {
        if (!fullscreen)
            this->setWindowState(Qt::WindowFullScreen);
        else
            this->setWindowState(Qt::WindowMaximized);
        fullscreen = !fullscreen;
    });

    display->addAction(full_screen);

    //

    this->setMenuBar(menu_bar);
}
void vinacomp::tool_bar()
{
	_tool_bar->setObjectName("tool_bar");

	_tool_bar->add_action(open_project_action);
	_tool_bar->add_action(new_project_action);

	_tool_bar->add_separator();

	_tool_bar->add_action(settings_action);
	_tool_bar->add_action(project_settings_action);

	_tool_bar->add_stretch();

	_tool_bar->add_action(script_layout_action);
	_tool_bar->add_action(comp_layout_action);

}

