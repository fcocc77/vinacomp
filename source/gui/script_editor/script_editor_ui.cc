#include <script_editor.h>
#include <qt.h>
#include <slider.h>

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *tools = tools_setup_ui();

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setObjectName("output");
    QCodeEditor *input = code_editor();

    layout->addWidget(tools);

    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(output);
    splitter->addWidget(input);

    splitter->setSizes({100, 100});

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
    connect(clear, &QPushButton::clicked, this, [this]() { output->clear(); });
    qt::set_icon(clear, "clear_script_a", icon_size);
    layout->addWidget(clear);

    QPushButton *run = new QPushButton();
    qt::set_icon(run, "run_script_a", icon_size);
    connect(run, &QPushButton::clicked, this, [this]() { run_script(); });
    layout->addWidget(run);

    layout->addStretch();

    QLabel *font_size_label = new QLabel("Font Size:");
    layout->addWidget(font_size_label);
    slider *font_size = new slider();
    layout->addWidget(font_size);

    return widget;
}

QCodeEditor *script_editor::code_editor()
{
    editor = new QCodeEditor();

    QPythonHighlighter *python_highlighter = new QPythonHighlighter();
    editor->setHighlighter(python_highlighter);

    connect(editor, &QTextEdit::textChanged, this, [this]() {
        // guarda el script escrito en el proyecto
        project->insert("script_editor", editor->toPlainText());
    });

    return editor;
}
