#include <vinacomp.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    vinacomp _vinacomp;
    _vinacomp.showMaximized();

    return a.exec();
}
