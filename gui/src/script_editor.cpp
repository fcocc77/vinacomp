#include <script_editor.hpp>

script_editor::script_editor(/* args */)
{
    this->setObjectName("script_editor");
    setup_ui();
}

script_editor::~script_editor()
{
}

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    QWidget *tools = tools_setup_ui();

    QPlainTextEdit *output = new QPlainTextEdit();
    output->setObjectName("output");
    kgl::QCodeEditor *input = new kgl::QCodeEditor();

    // esto es para que el editor tenga colores
    kgl::QCodeEditorDesign design("libs/QCodeEditor/files/design.xml");
    QList<kgl::QSyntaxRule> rules = kgl::QSyntaxRules::loadFromFile("libs/QCodeEditor/files/rule_cpp.xml", design);
    input->setDesign(design);
    input->setRules(rules);
    input->setKeywords({"printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset"});
    //

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
