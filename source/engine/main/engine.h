#ifndef ENGINE_H
#define ENGINE_H

#include <util.h>
#include <py_nodes.h>

class engine
{
private:
    QJsonObject *project;

public:
    engine(QString input_py);
    ~engine();
};

#endif // ENGINE_H