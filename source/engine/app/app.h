#ifndef APP_H
#define APP_H

#include <util.h>

class app
{
private:
public:
    app();
    ~app();

    void open_project(QString project_path);
};

#endif // APP_H