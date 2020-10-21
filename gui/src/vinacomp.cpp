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

    panels_layout *_layout = new panels_layout();

    // QWidget *central_widget = new QWidget();
    // central_widget->setObjectName("central_widget");
    // qt::add_widget(central_widget, _splitter);

    this->setCentralWidget(_layout);
}

void vinacomp::setup_style()
{
    QString style = fread("./resources/css/style.css");
    this->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana principal
    this->setWindowTitle("VinaComp");
}
