#ifndef APP_H
#define APP_H

#include <util.h>

class app
{
private:
    QJsonObject *project;

public:
    app( QJsonObject *_project );
    ~app();

    void open_project( QString project_path );
    void save_project( QString project_path );
};

#endif // APP_H