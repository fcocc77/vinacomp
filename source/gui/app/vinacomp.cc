#include <vinacomp.h>

vinacomp::vinacomp()
	: fullscreen(false)
	, project_settings_visible(false)
	, project_opened(false)
	, current_project("")
{
    this->setObjectName("vinacomp");
    project = new QJsonObject();
    setup_ui();
}

vinacomp::~vinacomp()
{
}

void vinacomp::setup_ui()
{
    _properties = new properties();
    _node_graph = new node_graph(project, _properties);
    _viewer = new viewer();
    _script_editor = new script_editor(project, _node_graph);
    _curve_editor = new curve_editor();
    _settings = new settings();
	_project_settings = new project_settings();

    _panels_layout = new panels_layout(_node_graph, _viewer, _script_editor, _properties, _curve_editor);
    _tool_bar = new tools(25, true);

    QWidget *central_widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central_widget);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(_panels_layout);
    layout->addWidget(_project_settings);
    layout->addWidget(_tool_bar);

    this->setCentralWidget(central_widget);

    update_sylesheet_action = new QAction("Update StyleSheet");
    update_sylesheet_action->setShortcut(QString("Ctrl+R"));
    connect(update_sylesheet_action, &QAction::triggered, this, &vinacomp::setup_style);
    setup_style();

    main_menu();
	tool_bar();
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
    settings_action->connect_to(this, [this]() {
        _settings->show();
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

QJsonArray vinacomp::get_recent_projects()
{
    QString recent_projects_path = "temp/recent_projects.json";
    QJsonArray recent_projects = jread(recent_projects_path)["projects"].toArray();

    return recent_projects;
}

void vinacomp::update_recent_projects()
{
    QJsonArray recent_projects = get_recent_projects();
    recent_projects_menu->clear();

    for (int i = 0; i < recent_projects.count(); i++)
    {
        QString project_path = recent_projects[i].toString();
        QAction *project_action = new QAction(project_path);
        project_action->setShortcut(QString("Alt+Shift+" + QString::number(i + 1)));

        connect(project_action, &QAction::triggered, this, [=]() {
            open_project(project_path);
        });

        recent_projects_menu->addAction(project_action);
    }
}

void vinacomp::recorder_recent_projects(QString project_path)
{
    QString recent_projects_path = "temp/recent_projects.json";
    int max_projects = 7;

    QJsonArray recent_projects = get_recent_projects();
    QJsonArray projects = {};

    // si el proyecto ya existe en la lista, lo elimina
    int index_delete = -1;
    for (int i = 0; i < recent_projects.count(); i++)
        if (recent_projects[i].toString() == project_path)
            index_delete = i;

    if (index_delete >= 0)
        recent_projects.removeAt(index_delete);
    //
    //
    recent_projects.push_front(project_path);

    if (recent_projects.count() < max_projects)
        max_projects = recent_projects.count();

    for (int i = 0; i < max_projects; i++)
        projects.push_back(recent_projects[i].toString());

    QJsonObject new_recent_projects;

    new_recent_projects.insert("projects", projects);
    jwrite(recent_projects_path, new_recent_projects);

    update_recent_projects();
}

void vinacomp::open_project_dialog()
{
    QFileDialog dialog(this);
    dialog.setDirectory(os::dirname(current_project));
    dialog.setFileMode(QFileDialog::AnyFile);

    dialog.setNameFilter(tr("VinaComp Project (*.vina)"));

    if (dialog.exec())
        open_project(dialog.selectedFiles()[0]);
}

void vinacomp::open_project(QString project_path)
{
    project_opened = true;
    current_project = project_path;
    recorder_recent_projects(project_path);

    (*project) = jread(project_path);

    // actualiza componentes con el proyecto cargado
    _node_graph->restore_tree(project->value("nodes").toObject());
    _node_graph->restore_scene_data(project->value("scene").toObject());
    _script_editor->open_script_from_project();

    this->setWindowTitle(os::basename(project_path) + " - VinaComp");
}

void vinacomp::to_save_project()
{
    if (!project_opened)
        save_as();
    else
        save_project(current_project);
}

void vinacomp::save_as()
{
    QFileDialog dialog(this);
    dialog.setDirectory(os::dirname(current_project));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("VinaComp Project (*.vina)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec())
    {
        QString project_path = dialog.selectedFiles()[0];

        if (!project_path.contains(".vina"))
            project_path += ".vina";

        save_project(project_path);
    }
}

void vinacomp::save_project(QString project_path)
{
    project->insert("nodes", _node_graph->get_tree());
    project->insert("scene", _node_graph->get_scene_data());

    recorder_recent_projects(project_path);
    jwrite(project_path, *project);

    project_opened = true;

    this->setWindowTitle(os::basename(project_path) + " - VinaComp");
}
