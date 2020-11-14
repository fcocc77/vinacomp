#include <curve_editor.hpp>

curve_editor ::curve_editor()
{
    this->setObjectName("curve_editor");
    setup_ui();

}

curve_editor ::~curve_editor()
{
}

void curve_editor::setup_ui()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QTreeWidget *knobs_tree = knobs_tree_setup_ui();

    layout->addWidget(knobs_tree);

    view = new curve_view();
    view->setObjectName("graphics_view");
    layout->addWidget(view);
    //
    //
}

QTreeWidget *curve_editor::knobs_tree_setup_ui()
{
    QTreeWidget *tree = new QTreeWidget();
    tree->setObjectName("knobs_tree");
    tree->setMaximumWidth(300);
    tree->setMinimumWidth(300);

    //
    //

    return tree;
}
