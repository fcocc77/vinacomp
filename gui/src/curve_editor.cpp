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
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    QTreeWidget *knobs_tree = knobs_tree_setup_ui();

    layout->addWidget(knobs_tree);

    view = new curve_view();
    view->setObjectName("graphics_view");

    tools *toolbar = new tools();
    toolbar->add_action(view->select_all_action);
    toolbar->add_action(view->delete_keyframes_action);
    toolbar->add_separator();
    toolbar->add_action(view->center_action);
    toolbar->add_stretch();
    toolbar->add_action(view->linear_action);
    toolbar->add_action(view->horizontal_action);
    toolbar->add_action(view->smooth_action);
    toolbar->add_action(view->break_action);

    QWidget *view_widget = new QWidget();
    QVBoxLayout *view_layout = new QVBoxLayout(view_widget);
    view_layout->setMargin(0);
    view_layout->setSpacing(0);

    view_layout->addWidget(toolbar);
    view_layout->addWidget(view);

    layout->addWidget(view_widget);
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
