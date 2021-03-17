#include <QCoreApplication>
#include <QString>
#include <engine.h>

int main( int argc, char *argv[] )
{
    // QCoreApplication app(argc, argv);

    QString input_py = argv[ 1 ];
    engine _engine( input_py );

    // return app.exec();
}
