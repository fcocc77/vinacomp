#include <splitter.hpp>

splitter::splitter(/* args */)
{

    this->setObjectName("splitter");

    QWidget *top_buttons = new QWidget();
    setup_top_buttons(top_buttons);

    tab_section = new QWidget();
    tab_section->setObjectName("tab_section");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(top_buttons);
    layout->addWidget(tab_section);

    this->setLayout(layout);
}

splitter::~splitter()
{
}

void splitter::setup_top_buttons(QWidget *top_buttons)
{

    top_buttons->setObjectName("top_buttons");
    top_buttons->setMaximumHeight(70);

    QPushButton *menu_button = new QPushButton();
    menu_button->setMaximumWidth(100);

    qt::set_icon(menu_button, "resources/images/layout.png");

    // Menu
    QAction *split_vertical = new QAction("split_vertical");
    connect(split_vertical, &QAction::triggered, this, [this]() {
        QWidget *parent = new QWidget();
        vertical_split(parent);
    });
    split_vertical->setIcon(QIcon("resources/images/split_vertically.png"));

    QAction *split_horizontal = new QAction("split_horizontal");
    connect(split_horizontal, &QAction::triggered, this, [this]() {
        QWidget *parent = new QWidget();
        horizontal_split(parent);
    });
    split_horizontal->setIcon(QIcon("resources/images/split_horizontally.png"));

    QMenu *menu = new QMenu(this);

    menu->addAction(split_vertical);
    menu->addAction(split_horizontal);
    menu->addSeparator();
    //
    //

    connect(menu_button, &QPushButton::clicked, this, [=]() {
        menu->popup(QCursor::pos());
        menu->show();
    });

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(menu_button);
    layout->addStretch();

    top_buttons->setLayout(layout);
}

void splitter::horizontal_split(QWidget *parent)
{
    print("horizontal_split");
}

void splitter::vertical_split(QWidget *parent)
{
    QSplitter *splitter_main = new QSplitter(this);
    splitter_main->setOrientation(Qt::Vertical);

    QSplitter *splitter_top = new QSplitter(this);
    QSplitter *splitter_bottom = new QSplitter(this);

    splitter *_splitter = new splitter();
    splitter_bottom->addWidget(_splitter);

    splitter_main->addWidget(splitter_top);
    splitter_main->addWidget(splitter_bottom);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(splitter_main);

    tab_section->setLayout(layout);
}