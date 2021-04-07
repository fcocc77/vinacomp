#include <knob_integer.h>
#include <qt.h>
#include <script_editor.h>
#include <slider.h>
#include <tools.h>

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *_tools = tools_setup_ui();

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setObjectName("output");
    QCodeEditor *input = code_editor();

    layout->addWidget(_tools);

    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(output);
    splitter->addWidget(input);

    splitter->setSizes({100, 100});

    layout->addWidget(splitter);
}

QWidget *script_editor::tools_setup_ui()
{
    tools *_tools = new tools();

    action *clear_action = new action("Clear", "", "clear_script");
    action *run_script_action = new action("Run Script", "", "run_script");

    clear_action->connect_to(this, [=]() { output->clear(); });
    run_script_action->connect_to(this, [=]() { run_script(); });

    knob_integer *font_size_slider = new knob_integer(1, 100, 14);
    connect(font_size_slider, &knob_integer::changed, this, [=](int value) {
        QString style = "font-size: " + QString::number(value) + "px;";
        output->setStyleSheet(style);
        editor->setStyleSheet(style);
    });
    font_size_slider->set_animatable(false);
    font_size_slider->set_init_space(100, "Font Size");

    _tools->add_action(clear_action);
    _tools->add_action(run_script_action);

    _tools->add_stretch();

    _tools->add_widget(font_size_slider);

    return _tools;
}

QCodeEditor *script_editor::code_editor()
{
    editor = new QCodeEditor();
    editor->setObjectName("code_editor");

    QPythonHighlighter *python_highlighter = new QPythonHighlighter();
    editor->setHighlighter(python_highlighter);

    connect(editor, &QTextEdit::textChanged, this, [this]() {
        // guarda el script escrito en el proyecto
        project->insert("script_editor", editor->toPlainText());
    });

    return editor;
}
