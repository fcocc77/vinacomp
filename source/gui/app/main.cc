#include <vinacomp.h>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    vinacomp _vinacomp;
    _vinacomp.showMaximized();

    return app.exec();
}
