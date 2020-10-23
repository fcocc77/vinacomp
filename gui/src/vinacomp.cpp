#include <vinacomp.hpp>

vinacomp::vinacomp(QWidget *parent) : QMainWindow(parent)
{
    setup_ui();
    setup_style();
}

vinacomp::~vinacomp()
{
}

void vinacomp::setup_ui()
{
    update_sylesheet_action = new QAction("Update StyleSheet");
    update_sylesheet_action->setShortcut(QString("Ctrl+R"));
    connect(update_sylesheet_action, &QAction::triggered, this, &vinacomp::setup_style);

    _node_graph = new node_graph();
    _viewer = new viewer();
    _script_editor = new script_editor();
    _properties = new properties();
    _curve_editor = new curve_editor();

    _panels_layout = new panels_layout(_node_graph, _viewer, _script_editor, _properties, _curve_editor);

    _tool_bar = new tool_bar();

    QWidget *central_widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    central_widget->setLayout(layout);

    layout->addWidget(_tool_bar);
    layout->addWidget(_panels_layout);

    this->setCentralWidget(central_widget);

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

    QMenu *edit_menu = new QMenu("Edit", menu_bar);
    menu_bar->addMenu(edit_menu);
    edit_menu->addAction(update_sylesheet_action);

    QMenu *layout_menu = new QMenu("Layout", menu_bar);
    menu_bar->addMenu(layout_menu);
    layout_menu->addAction(_panels_layout->restore_default_action);
    layout_menu->addAction(_panels_layout->save_current_action);

    this->setMenuBar(menu_bar);
}
