#include <script_editor.hpp>

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

    splitter->setSizes({100, 300});

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
    connect(run, &QPushButton::clicked, this, [this]() {
    });
    layout->addWidget(run);

    layout->addStretch();

    QLabel *font_size_label = new QLabel("Font Size:");
    layout->addWidget(font_size_label);
    slider *font_size = new slider();
    layout->addWidget(font_size);

    return widget;
}
