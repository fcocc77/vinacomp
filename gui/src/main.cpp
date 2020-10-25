#include <vinacomp.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    vinacomp _vinacomp(&app);
    _vinacomp.showMaximized();

    return app.exec();
}
