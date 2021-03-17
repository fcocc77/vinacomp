#include <app.h>

app::app(QJsonObject *_project)
    : project(_project)
{
}

app::~app() {}

void app::open_project(QString project_path)
{
    print(project_path);
}

void app::save_project(QString project_path)
{
    jwrite(project_path, *project);
}