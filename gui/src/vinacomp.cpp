#include <vinacomp.hpp>

vinacomp::vinacomp(QApplication *_app)
{
    app = _app;
    project = new QJsonObject();
    setup_ui();
}

vinacomp::~vinacomp()
{
}

void vinacomp::setup_ui()
{

    _node_graph = new node_graph(project);
    _viewer = new viewer();
    _script_editor = new script_editor(project);
    _properties = new properties();
    _curve_editor = new curve_editor();
    _settings = new settings();

    _panels_layout = new panels_layout(app, _node_graph, _viewer, _script_editor, _properties, _curve_editor);

    _tool_bar = new tool_bar();

    QWidget *central_widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    central_widget->setLayout(layout);

    layout->addWidget(_tool_bar);
    layout->addWidget(_panels_layout);

    this->setCentralWidget(central_widget);

    update_sylesheet_action = new QAction("Update StyleSheet");
    update_sylesheet_action->setShortcut(QString("Ctrl+R"));
    connect(update_sylesheet_action, &QAction::triggered, this, &vinacomp::setup_style);
    setup_style();

    main_menu();
}

void vinacomp::setup_style()
{
    QString style = fread("./resources/css/style.css");
    this->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana principal
    this->setWindowTitle("VinaComp");
}

void vinacomp::main_menu()
{

    QMenuBar *menu_bar = new QMenuBar();

    QMenu *file_menu = new QMenu("File", menu_bar);
    menu_bar->addMenu(file_menu);

    QAction *new_project = new QAction("New Project");
    new_project->setShortcut(QString("Ctrl+N"));
    new_project->setIcon(QIcon("resources/images/add_a.png"));
    file_menu->addAction(new_project);

    QAction *open_project_action = new QAction("Open Project");
    open_project_action->setIcon(QIcon("resources/images/folder_a.png"));
    open_project_action->setShortcut(QString("Ctrl+O"));
    connect(open_project_action, &QAction::triggered, this, &vinacomp::open_project_dialog);

    file_menu->addAction(open_project_action);

    recent_projects_menu = new QMenu("Recent Projects", this);
    update_recent_projects();
    file_menu->addMenu(recent_projects_menu);

    file_menu->addSeparator();

    QAction *save_project_action = new QAction("Save Project");
    save_project_action->setIcon(QIcon("resources/images/save_a.png"));
    save_project_action->setShortcut(QString("Ctrl+S"));
    connect(save_project_action, &QAction::triggered, this, &vinacomp::to_save_project);
    file_menu->addAction(save_project_action);

    QAction *save_project_as = new QAction("Save Project As...");
    save_project_as->setShortcut(QString("Ctrl+Shift+S"));
    connect(save_project_as, &QAction::triggered, this, &vinacomp::save_as);
    file_menu->addAction(save_project_as);

    file_menu->addSeparator();

    QAction *quit = new QAction("Quit");
    quit->setIcon(QIcon("resources/images/quit_a.png"));
    quit->setShortcut(QString("Ctrl+Q"));
    connect(quit, &QAction::triggered, this, [this]() {
        this->close();
    });
    file_menu->addAction(quit);
    //
    //

    //
    QMenu *edit_menu = new QMenu("Edit", menu_bar);
    menu_bar->addMenu(edit_menu);

    QAction *settings_action = new QAction("Settings...");
    connect(settings_action, &QAction::triggered, this, [this]() {
        _settings->show();
    });

    settings_action->setShortcut(QString("Shift+S"));
    settings_action->setIcon(QIcon("resources/images/settings_a.png"));

    edit_menu->addAction(settings_action);

    edit_menu->addAction(update_sylesheet_action);
    //
    //

    //
    QMenu *layout_menu = new QMenu("Layout", menu_bar);
    menu_bar->addMenu(layout_menu);
    layout_menu->addAction(_panels_layout->restore_default_action);
    _panels_layout->restore_default_action->setIcon(QIcon("resources/images/layout_a.png"));

    layout_menu->addAction(_panels_layout->save_current_action);
    _panels_layout->save_current_action->setIcon(QIcon("resources/images/save_a.png"));

    //
    //

    QMenu *display = new QMenu("Display", menu_bar);
    menu_bar->addMenu(display);

    QAction *full_screen = new QAction("Full Screen");
    connect(full_screen, &QAction::triggered, this, [this]() {
        if (!fullscreen)
            this->setWindowState(Qt::WindowFullScreen);
        else
            this->setWindowState(Qt::WindowMaximized);
        fullscreen = !fullscreen;
    });

    full_screen->setShortcut(QString("Alt+S"));
    full_screen->setIcon(QIcon("resources/images/fullscreen_a.png"));

    display->addAction(full_screen);

    //

    this->setMenuBar(menu_bar);
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
}
