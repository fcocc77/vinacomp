#include <script_editor.h>
#include <action.h>

script_editor::script_editor(QJsonObject *_project, QWidget *_node_graph,
                             bool _expression_editor)
    : project(_project)
    , node_graph(_node_graph)
    , expression_editor(_expression_editor)
    , expression_editor_bar(nullptr)
    , exp_label(nullptr)
    , current_knob(nullptr)
    , current_group(nullptr)
{

    this->setObjectName("script_editor");
    setup_ui();

    action *run = new action("Run Script", "Ctrl+Return", "");
    run->connect_to(editor, [this]() { run_script_from_editor(); });

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

void script_editor::run_script_from_editor(bool output_log)
{
    QString script = editor->toPlainText();

    if (output_log)
        run_script(script);
    else
        python_run(script);
}

void script_editor::run_script(QString script, bool input_script_log)
{
    QString out = python_run(script);

    if (input_script_log)
        append_output(editor->toPlainText(), {100, 100, 100});

    if (out.contains("Error:"))
        append_output(out, {200, 0, 0});
    else
    {
        if (out.isEmpty())
            append_output("...", {50, 150, 80});
        else
            append_output("Result:", {50, 150, 80});
        append_output(out, {100, 150, 200});
    }

    output->verticalScrollBar()->setValue(100000000);
}
