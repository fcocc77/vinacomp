#include <script_editor.h>

script_editor::script_editor(QJsonObject *_project, QWidget *_node_graph)
    : project(_project)
    , node_graph(_node_graph)
{

    this->setObjectName("script_editor");
    setup_ui();

    action *run = new action("Run Script", "Ctrl+Return", "");
    run->connect_to(editor, [this]() { run_script(); });

    python_initialize();
}

script_editor::~script_editor()
{
    Py_Finalize();
}

void script_editor::open_script_from_project()
{
    // abre el script guardado en el proyecto
    QString texts = project->take("script_editor").toString();
    editor->setPlainText(texts);
}

void script_editor::append_output(QString text, QColor color)
{
    QColor text_color = output->textColor();

    output->setTextColor(color);
    output->append(text);

    output->setTextColor(text_color);
}

void script_editor::run_script()
{
    QString out = python_run(editor->toPlainText());

    append_output(editor->toPlainText(), {100, 100, 100});

    if (out.contains("Error:"))
        append_output(out, {200, 0, 0});
    else
    {
        append_output("\nResult:", {50, 150, 80});
        append_output(out, {100, 150, 200});
    }

    output->verticalScrollBar()->setValue(100000000);
}