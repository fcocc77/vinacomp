#include <QApplication>
#include <vinacomp.h>
#include <global.h>
#include <os.h>

int main(int argc, char *argv[])
{
    os::makedir(VINACOMP_CONF_PATH);

    QApplication app(argc, argv);
    vinacomp _vinacomp;
    _vinacomp.show();
    // _vinacomp.showMaximized();

    return app.exec();
}
