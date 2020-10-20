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

    _splitter = new splitter();
    this->setCentralWidget(_splitter);
}

void vinacomp::setup_style()
{
    QString style = fread("./resources/css/style.css");
    this->setStyleSheet(style.toStdString().c_str());

    // Titulo de ventana principal
    this->setWindowTitle("VinaComp");
}
