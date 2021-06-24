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

    connect(this, &QTreeWidget::itemClicked, this, [=]() {
        clicked(get_selected_param_items());
    });
}

QList<clicked_param> curve_tree::get_selected_param_items() const
{
    // obtiene los items seleccionados 'params', y si el item es el padre
    // 'node_name' inserta todos los hijos a la lista

    auto get_param = [](QTreeWidgetItem *item, QString node_name) {
        QStringList param_dimension_name = item->text(0).split('.');

        QString param_name = param_dimension_name[0];
        QString dimension_name;

        if (param_dimension_name.count() == 2)
            dimension_name = param_dimension_name[1];

        return clicked_param{node_name, param_name, dimension_name};
    };

    QList<clicked_param> selected_list;

    auto contains_param = [&](clicked_param param) {
        for (auto _param : selected_list)
            if (param.name == _param.name &&
                param.node_name == _param.node_name &&
                param.dimension_name == _param.dimension_name)
                return true;

        return false;
    };

    for (auto *item : selectedItems())
    {
        auto parent = item->parent();

        if (!parent)
        {
            for (auto child : get_children(item))
            {
                QString node_name = item->text(0);
                auto param = get_param(child, node_name);

                if (!contains_param(param))
                    selected_list.push_back(param);

                child->setSelected(true);
            }

            continue;
        }
        else
        {
            QString node_name = parent->text(0);
            auto param = get_param(item, node_name);

            if (!contains_param(param))
                selected_list.push_back(param);
        }
    }

    return selected_list;
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
                          QString dimension_name, QColor color)
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

    QString param_dimension_name = param_name;

    if (!dimension_name.isEmpty())
        param_dimension_name += '.' + dimension_name;

    QTreeWidgetItem *param_item =
        get_param_item(node_item, param_dimension_name);

    if (!param_item)
    {
        param_item = new QTreeWidgetItem(node_item);
        param_item->setText(0, param_dimension_name);

        node_item->addChild(param_item);
        param_item->setExpanded(true);
    }
}

void curve_tree::delete_item(QString node_name, QString param_name,
                             QString dimension_name)
{
    QTreeWidgetItem *node_item = get_node_item(node_name);
    if (!node_item)
        return;

    QString param_dimension_name = param_name;

    if (!dimension_name.isEmpty())
        param_dimension_name += '.' + dimension_name;

    QTreeWidgetItem *param_item = get_param_item(node_item, param_dimension_name);

    if (param_item)
        delete param_item;

    if (param_name.isEmpty() || !node_item->childCount())
    {
        this->invisibleRootItem()->removeChild(node_item);
        delete node_item; // al eliminar item 'delete', elimina todos los hijos
                          // ya comprobado
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
