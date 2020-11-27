#ifndef ENGINE_H
#define ENGINE_H

#include <util.h>
#include <py_nodes.h>
#include <py_app.h>
#include <app.h>

class engine
{
private:
    app *_app;
    QJsonObject *project;
    QString input_py;

    void python_initialize();

public:
    engine(QString _input_py);
    ~engine();
};

#endif // ENGINE_H