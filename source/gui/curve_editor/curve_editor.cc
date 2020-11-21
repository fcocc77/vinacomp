#include <curve_editor.h>

curve_editor ::curve_editor()
{
    this->setObjectName("curve_editor");
    setup_ui();

    add_curve();
    add_curve();
}

curve_editor ::~curve_editor()
{
}

void curve_editor::setup_ui()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    QTreeWidget *knobs_tree = knobs_tree_setup_ui();

    layout->addWidget(knobs_tree);

    view = new curve_view();
    view->setObjectName("graphics_view");

    QLineEdit *expression_curve = new QLineEdit();
    QLabel *expression_result = new QLabel(" = 20");
    QLabel *expression_name = new QLabel("Transform.translate.x");

    tools *toolbar = new tools();
    toolbar->add_action(view->select_all_action);
    toolbar->add_action(view->delete_keyframes_action);
    toolbar->add_separator();
    toolbar->add_action(view->center_action);
    toolbar->add_stretch();
    toolbar->add_widget(expression_name);
    toolbar->add_widget(expression_curve);
    toolbar->add_widget(expression_result);
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
    tree = new QTreeWidget();
    tree->setObjectName("knobs_tree");
    tree->setMaximumWidth(300);
    tree->setMinimumWidth(300);

    tree->setHeaderHidden(true);

    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setAlternatingRowColors(true);

    return tree;
}

void curve_editor::add_item(QString node, QString param, QString dimension)
{
    QTreeWidgetItem *node_item = new QTreeWidgetItem();
    node_item->setText(0, "Transform");

    QTreeWidgetItem *param_item = new QTreeWidgetItem();
    param_item->setText(0, "translate");

    QTreeWidgetItem *dimension_item = new QTreeWidgetItem();
    dimension_item->setText(0, "x");

    node_item->addChild(param_item);
    param_item->addChild(dimension_item);

    tree->addTopLevelItem(node_item);

    node_item->setExpanded(true);
    param_item->setExpanded(true);
}

void curve_editor::add_curve()
{
    QString name = "Grade";
    QString param = "white";
    QString dimension = "r";

    QJsonArray keys;
    keys.insert(0, {{0.0, 1.0, 0, 0, false}});
    keys.insert(1, {{1.0, 0.5, 0, 0, false}});
    keys.insert(2, {{3.0, 1.5, 0, 0, false}});
    keys.insert(3, {{4.0, 0.1, 0, 0, false}});

    this->add_item(name, param, dimension);
    view->create_curve(name, Qt::cyan, keys);
}