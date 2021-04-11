#include <curve_tree.h>
#include <util.h>

curve_tree::curve_tree()
{
    this->setObjectName("knobs_tree");
    this->setMaximumWidth(300);
    this->setMinimumWidth(300);

    this->setHeaderHidden(true);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setAlternatingRowColors(true);

    connect(this, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item) {
        auto parent = item->parent();

        // si se selecciono el padre, envia una lista con todos
        // los parametros y retorna
        if (!parent)
        {
            QList<QString> params_name;
            for (auto child : get_children(item))
            {
                params_name.push_back(child->text(0));
                child->setSelected(true);
            }
            clicked(item->text(0), params_name);
            return;
        }

        QString node_name = parent->text(0);
        QString param_name = item->text(0);
        clicked(node_name, {param_name});
    });
}

QList<QTreeWidgetItem *> curve_tree::get_children(QTreeWidgetItem *item) const
{
    QList<QTreeWidgetItem *> children;
    for (int i = 0; i < item->childCount(); i++)
        children.push_back(item->child(i));

    return children;
}

QTreeWidgetItem *curve_tree::get_node_item(QString item_name) const
{
    // obtiene los hijos de un item, 'QTreeWidgetItem' tiene una
    // opcion oficial 'takeChildren()' pero tiene un error al eliminar los
    // items.
    auto items = this->findItems(item_name, Qt::MatchExactly);
    for (QTreeWidgetItem *item : items)
        if (item->text(0) == item_name)
            return item;

    return nullptr;
}

QTreeWidgetItem *curve_tree::get_param_item(QTreeWidgetItem *node_item,
                                            QString param_name) const
{
    for (auto child : get_children(node_item))
        if (child->text(0) == param_name)
            return child;

    return nullptr;
}

void curve_tree::add_item(QString node_name, QString param_name,
                          QString dimension, QColor color)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (!node_item)
    {
        node_item = new QTreeWidgetItem();
        node_item->setText(0, node_name);

        this->addTopLevelItem(node_item);
        node_item->setExpanded(true);
    }
    node_item->setBackgroundColor(0, color);

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

void curve_tree::delete_item(QString node_name)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (node_item)
    {
        this->invisibleRootItem()->removeChild(node_item);

        // al eliminar item 'delete', elimina todos los hijos (ya comprobado)
        delete node_item;
    }
}

void curve_tree::mousePressEvent(QMouseEvent *event)
{
    // envia una señal si el click se hizo fuera de un item
    QModelIndex item = indexAt(event->pos());
    if (!item.isValid())
        outside_clicked();

    QTreeView::mousePressEvent(event);
}
