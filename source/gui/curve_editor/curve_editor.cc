#include <animation.h>
#include <curve_editor.h>
#include <tools.h>

curve_editor ::curve_editor()
{
    this->setObjectName("curve_editor");
    setup_ui();
}

curve_editor ::~curve_editor() {}

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

QTreeWidgetItem *curve_editor::get_node_item(QString item_name) const
{
    auto items = tree->findItems(item_name, Qt::MatchExactly);
    for (QTreeWidgetItem *item : items)
        if (item->text(0) == item_name)
            return item;

    return nullptr;
}

QTreeWidgetItem *curve_editor::get_param_item(QTreeWidgetItem *node_item,
                                              QString param_name) const
{
    for (int i = 0; i < node_item->childCount(); i++)
    {
        auto item = node_item->child(i);
        if (item->text(0) == param_name)
            return item;
    }

    return nullptr;
}

void curve_editor::add_item(QString node_name, QString param_name,
                            QString dimension)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (!node_item)
    {
        node_item = new QTreeWidgetItem();
        node_item->setText(0, node_name);

        tree->addTopLevelItem(node_item);
        node_item->setExpanded(true);
    }

    QTreeWidgetItem *param_item = get_param_item(node_item, param_name);
    if (!param_item)
    {
        param_item = new QTreeWidgetItem(node_item);
        param_item->setText(0, param_name);

        node_item->addChild(param_item);
        param_item->setExpanded(true);
    }

    // QTreeWidgetItem *dimension_item = new QTreeWidgetItem();
    // dimension_item->setText(0, "x");

    // param_item->addChild(dimension_item);
}

void curve_editor::delete_node_item(QString node_name)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (node_item)
    {
        tree->invisibleRootItem()->removeChild(node_item);

        // al eliminar item 'delete', elimina todos los hijos (ya comprobado)
        delete node_item;
    }
}

void curve_editor::update_from_trim_panel(trim_panel *panel)
{
    QString node_name = panel->get_name();

    delete_node_item(node_name);

    for (QString key : panel->get_knobs()->keys())
    {
        knob *_knob = panel->get_knobs()->value(key);
        if (!_knob->is_animated())
            continue;

        QString param_name = _knob->get_name();

        QString dimension = "r";
        // QString curve = "f0 20 l5 r7 f10 20.9 f30 5";

        this->add_item(node_name, param_name, dimension);
        // view->create_curve(name, Qt::cyan, anim::convert_curve(curve));
    }
}

void curve_editor::delete_curve(QString node_name)
{
    delete_node_item(node_name);
    // view->delete_curve(node_name);
}
