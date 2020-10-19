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

    // Splitter
    QSplitter *splitter_main = new QSplitter(this);
    splitter_main->setOrientation(Qt::Vertical);
    this->setCentralWidget(splitter_main);

    QSplitter *splitter_top = new QSplitter(this);
    QSplitter *splitter_bottom = new QSplitter(this);
    //
    //

    _viewer = new viewer();
    _node_graph = new node_graph();

    splitter_top->addWidget(_viewer);
    splitter_bottom->addWidget(_node_graph);

    splitter_main->addWidget(splitter_top);
    splitter_main->addWidget(splitter_bottom);

    splitter_main->setSizes({100, 100});
}

void vinacomp::setup_style()
{

    // Estilo de general
    QString style = fread("./resources/css/style.css");
    this->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana principal
    this->setWindowTitle("VinaComp");
}
