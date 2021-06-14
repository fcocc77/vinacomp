#include <util.h>
#include <vinacomp.h>

vinacomp::vinacomp()
    : fullscreen(false)
    , project_settings_visible(false)
    , settings_visible(false)
    , project_opened(false)
    , current_project("")
    , _settings(0)
{
    this->setObjectName("vinacomp");
    project_old = new QJsonObject();
    project = new project_struct();

    // Viewer Vacio
    QLabel *empty_viewer = new QLabel("( Viewer1 )");
    empty_viewer->setAlignment(Qt::AlignCenter);
    empty_viewer->setObjectName("empty_viewer");
    viewers = new QMap<QString, viewer *>;
    viewers_gl = new QList<QWidget *>;
    //

    // node_graph de gurpos
    groups_node_graph = new QMap<QString, node_graph *>;
    //
    //

    _file_dialog = new file_dialog(this);

    _settings = new general_settings(this);
    _project_settings = new project_settings(this);

    _properties = new properties(this);
    _node_graph = new node_graph(this, project, _properties);
    _curve_editor = new curve_editor(this);
    _script_editor =
        new script_editor(project_old, _node_graph, false, _settings, this);

    _panels_layout =
        new panels_layout(this, _node_graph, empty_viewer, _script_editor,
                          _properties, _curve_editor);
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
    connect(update_sylesheet_action, &QAction::triggered, this,
            &vinacomp::setup_style);
    setup_style();

    main_menu();
    tool_bar();

    _renderer = new renderer(project);
}

vinacomp::~vinacomp() {}

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
    recent_projects_menu = new QMenu("Recent Projects", this);
    QMenu *edit_menu = new QMenu("Edit", menu_bar);
    QMenu *layout_menu = new QMenu("Layout", menu_bar);
    QMenu *display = new QMenu("Display", menu_bar);

    // Acciones
    new_project_action = new action("New Project", "Ctrl+N", "add");
    open_project_action = new action("Open Project", "Ctrl+O", "folder");
    action *save_project_action = new action("Save Project", "Ctrl+S", "save");
    action *save_project_as = new action("Save Project As...", "Ctrl+Shift+S");
    action *quit = new action("Quit", "Ctrl+Q", "quit");
    settings_action = new action("Settings...", "Shift+S", "settings");
    project_settings_action =
        new action("Project Settings", "S", "project_settings");
    action *undo_action = new action("Undo", "Ctrl+Z", "");
    action *redo_action = new action("Redo", "Ctrl+Shift+Z", "");
    action *full_screen = new action("Full Screen", "Alt+S", "fullscreen");

    // Opciones
    settings_action->set_checkable(true);
    project_settings_action->set_checkable(true);
    _panels_layout->restore_default_action->setIcon(
        QIcon("resources/images/layout_a.png"));

    _panels_layout->save_current_action->setIcon(
        QIcon("resources/images/save_a.png"));

    update_recent_projects();

    // Conecciones
    new_project_action->connect_to(this, [this]() { new_project(); });
    open_project_action->connect_to(this, [this]() { open_project_dialog(); });
    save_project_action->connect_to(this, [this]() { to_save_project(); });
    save_project_as->connect_to(this, [this]() { save_as(); });
    quit->connect_to(this, [this]() { this->close(); });

    settings_action->connect_to(this, [this]() {
        settings_visible = !settings_visible; // Toggle
        _settings->setVisible(settings_visible);
    });

    project_settings_action->connect_to(this, [this]() {
        project_settings_visible = !project_settings_visible; // Toggle
        _project_settings->setVisible(project_settings_visible);
    });

    undo_action->connect_to(this, [this]() { undo(); });
    redo_action->connect_to(this, [this]() { redo(); });

    full_screen->connect_to(this, [this]() {
        if (!fullscreen)
            this->setWindowState(Qt::WindowFullScreen);
        else
            this->setWindowState(Qt::WindowMaximized);
        fullscreen = !fullscreen;
    });

    // Layout
    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(edit_menu);
    menu_bar->addMenu(layout_menu);
    menu_bar->addMenu(display);

    file_menu->addAction(new_project_action);
    file_menu->addAction(open_project_action);
    file_menu->addMenu(recent_projects_menu);
    file_menu->addSeparator();
    file_menu->addAction(save_project_action);
    file_menu->addAction(save_project_as);
    file_menu->addSeparator();
    file_menu->addAction(quit);

    edit_menu->addAction(settings_action);
    edit_menu->addAction(project_settings_action);
    edit_menu->addAction(update_sylesheet_action);
    edit_menu->addSeparator();
    edit_menu->addAction(undo_action);
    edit_menu->addAction(redo_action);

    layout_menu->addAction(_panels_layout->script_layout_action);
    layout_menu->addAction(_panels_layout->comp_layout_action);
    layout_menu->addAction(_panels_layout->all_visible_layout_action);
    layout_menu->addSeparator();
    layout_menu->addAction(_panels_layout->restore_default_action);
    layout_menu->addAction(_panels_layout->save_current_action);

    display->addAction(full_screen);

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

    _tool_bar->add_action(_panels_layout->restore_default_action);
    _tool_bar->add_action(_panels_layout->script_layout_action);
    _tool_bar->add_action(_panels_layout->comp_layout_action);
    _tool_bar->add_action(_panels_layout->all_visible_layout_action);
}

void vinacomp::update_render_all_viewer(bool clear_init_image) const
{
    for (viewer *_viewer : *viewers)
        _viewer->update_render(clear_init_image);
}

QMap<QString, node_graph *> vinacomp::get_all_node_graph() const
{
    QMap<QString, node_graph *> all_node_garph;
    all_node_garph = *get_groups_node_graph();

    all_node_garph.insert("main_node_graph", get_main_node_graph());

    return all_node_garph;
}

node_graph *vinacomp::get_node_graph(QString node_graph_name) const
{
    node_graph *__node_graph = get_all_node_graph().value(node_graph_name);
    if (__node_graph)
        return __node_graph;

    return get_main_node_graph();
}
