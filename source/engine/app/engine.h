#ifndef ENGINE_H
#define ENGINE_H

#include <app.h>
#include <nodes.h>
#include <py_app.h>
#include <py_nodes.h>
#include <util.h>

class engine
{
private:
    app *_app;
    nodes *_nodes;
    QJsonObject *project;
    QString input_py;

    void python_initialize();

public:
    engine(QString _input_py);
    ~engine();
};

#endif // ENGINE_H