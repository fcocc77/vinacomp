#include <script_editor.hpp>

script_editor::script_editor(/* args */)
{
    this->setObjectName("script_editor");
    setup_ui();
}

script_editor::~script_editor()
{
}

QCodeEditor *script_editor::code_editor()
{
    QCodeEditor *editor = new QCodeEditor();

    QPythonHighlighter *python_highlighter = new QPythonHighlighter();
    editor->setHighlighter(python_highlighter);

    return editor;
}

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    QWidget *tools = tools_setup_ui();

    QPlainTextEdit *output = new QPlainTextEdit();
    output->setObjectName("output");
    QCodeEditor *input = code_editor();

    layout->addWidget(tools);

    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(output);
    splitter->addWidget(input);

    layout->addWidget(splitter);
}

QWidget *script_editor::tools_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("tools");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);

    int icon_size = 20;

    QPushButton *clear = new QPushButton();
    qt::set_icon(clear, "clear_script_a", icon_size);
    layout->addWidget(clear);

    QPushButton *run = new QPushButton();
    qt::set_icon(run, "run_script_a", icon_size);
    layout->addWidget(run);

    layout->addStretch();

    return widget;
}
