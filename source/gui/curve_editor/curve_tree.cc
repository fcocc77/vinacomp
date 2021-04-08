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
        if (!parent)
            return;

        QString node_name = parent->text(0);
        QString param_name = item->text(0);

        clicked(node_name, param_name);
    });
}

QTreeWidgetItem *curve_tree::get_node_item(QString item_name) const
{
    auto items = this->findItems(item_name, Qt::MatchExactly);
    for (QTreeWidgetItem *item : items)
        if (item->text(0) == item_name)
            return item;

    return nullptr;
}

QTreeWidgetItem *curve_tree::get_param_item(QTreeWidgetItem *node_item,
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

void curve_tree::add_item(QString node_name, QString param_name,
                          QString dimension)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (!node_item)
    {
        node_item = new QTreeWidgetItem();
        node_item->setText(0, node_name);

        this->addTopLevelItem(node_item);
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
