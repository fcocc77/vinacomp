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

    _layout = new panels_layout(_node_graph, _viewer);

    // QWidget *central_widget = new QWidget();
    // central_widget->setObjectName("central_widget");
    // qt::add_widget(central_widget, _splitter);

    this->setCentralWidget(_layout);

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
    layout_menu->addAction(_layout->restore_default_action);
    layout_menu->addAction(_layout->save_current_action);

    this->setMenuBar(menu_bar);
}
