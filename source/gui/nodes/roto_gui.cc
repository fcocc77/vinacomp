#include <roto_gui.h>

roto_gui::roto_gui()
    : tree(new QTreeWidget)
{
    tree->setAlternatingRowColors(true);
    QStringList columns{"Label", "Visibility", "Lock"};
    tree->setHeaderLabels(columns);
    tree->setColumnWidth(0, 100);
    tree->setColumnWidth(1, 100);
    tree->setColumnWidth(2, 100);

    buttons = new QWidget;
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons);

    add_button = new button();
    remove_button = new button();

    add_button->set_icon("add");
    remove_button->set_icon("delete");

    buttons_layout->addWidget(add_button);
    buttons_layout->addWidget(remove_button);
    buttons_layout->addStretch();
}

roto_gui::~roto_gui()
{
    delete tree;
    delete add_button;
    delete remove_button;
}

void roto_gui::setup_knobs(QMap<QString, QVBoxLayout *> layouts)
{
    QVBoxLayout *controls_layout = layouts.value("Controls");

    controls_layout->addWidget(tree);
    controls_layout->addWidget(buttons);
}

void roto_gui::changed(knob *_knob) {}
