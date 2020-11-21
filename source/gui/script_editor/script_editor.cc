#include <script_editor.h>

script_editor::script_editor(QJsonObject *_project)
    : project(_project)
{

    this->setObjectName("script_editor");
    setup_ui();
}

script_editor::~script_editor()
{
}

void script_editor::open_script_from_project()
{
    // abre el script guardado en el proyecto
    QString texts = project->take("script_editor").toString();
    editor->setPlainText(texts);
}
