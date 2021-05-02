#include <QHeaderView>
#include <node_finder.h>
#include <qt.h>
#include <util.h>

node_finder::node_finder(QWidget *__node_view, nodes_load *_nodes)
    : QWidget(__node_view)
    , nodes(_nodes)
    , _node_view(__node_view)
    , size(QSize(200, 200))
{
    this->setObjectName("node_finder");
    this->setMinimumSize(size);

    search_field = new QLineEdit(this);
    connect(search_field, &QLineEdit::textChanged, this, [=]() {
        if (search_field->hasFocus())
            update_tree();
    });

    tree = new QTreeWidget(this);
    tree->setHeaderHidden(true);
    tree->setAlternatingRowColors(true);
    connect(tree, &QTreeWidget::itemDoubleClicked, [=]() { create_node(); });
    connect(
        tree, &QTreeWidget::currentItemChanged, this,
        [=](QTreeWidgetItem *item) { search_field->setText(item->text(0)); });

    layout = new QVBoxLayout();
    this->setLayout(layout);

    layout->addWidget(search_field);
    layout->addWidget(tree);

    for (QJsonValue value : nodes->get_effects())
    {
        QString node_name = value.toObject()["label"].toString();
        QString node_id = value.toObject()["id"].toString();
        QString icon_name = value.toObject()["icon"].toString();
        QTreeWidgetItem *item = new QTreeWidgetItem();

        if (node_name.isEmpty())
            continue;

        item->setText(0, node_name);
        item->setText(1, node_id);
        if (icon_name.contains("/"))
            item->setIcon(0, QIcon(icon_name));
        else
            item->setIcon(0, QIcon("resources/images/" + icon_name + "_a.png"));
        tree->addTopLevelItem(item);
    }

    qt::shortcut("Tab", _node_view, [this]() { this->show_finder(); });
    qt::shortcut("Escape", _node_view, [this]() { this->hide(); });

    this->hide();
}

node_finder::~node_finder()
{
    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);
        delete item;
    }

    delete tree;
    delete search_field;
}

void node_finder::select_first_item()
{
    // selecciona el primer item visible
    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);
        if (!item->isHidden())
        {
            item->setSelected(true);
            return;
        }
    }
}

QTreeWidgetItem *node_finder::get_item(QString key) const
{
    key = key.toUpper();

    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);
        if (item->text(0).toUpper().contains(key))
            return item;
    }

    return nullptr;
}

void node_finder::update_tree()
{
    QString text = search_field->text().toUpper();

    tree->clearSelection();

    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);

        if (item->text(0).toUpper().contains(text))
            item->setHidden(false);
        else
            item->setHidden(true);

        if (text.isEmpty())
            item->setHidden(false);
    }

    select_first_item();
}

void node_finder::show_finder()
{
    if (isVisible())
    {
        if (search_field->hasFocus())
            create_node();
        else
            this->set_focus();
        return;
    }

    search_field->setText(last_node_created);
    search_field->selectAll();
    update_tree();

    this->set_focus();
    QPoint position = _node_view->mapFromGlobal(QCursor::pos());

    bool reverse = position.y() + 100 > _node_view->height();
    int mid_width = 130;

    if (reverse)
    {
        layout->setDirection(QBoxLayout::BottomToTop);
        this->move(position.x() - mid_width, position.y() - 240);
    }
    else
    {
        layout->setDirection(QBoxLayout::TopToBottom);
        this->move(position.x() - mid_width, position.y());
    }
    this->show();
}

void node_finder::create_node()
{
    if (search_field->text().isEmpty())
        return;

    auto item = get_item(search_field->text());
    if (!item)
        return;

    QString node_id = item->text(1);
    QString node_name = item->text(0);

    if (node_id.isEmpty())
        return;

    created(node_id);
    last_node_created = node_name;
    hide();
}

void node_finder::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up)
        tree->setFocus();
    else if (event->key() == Qt::Key_Return)
        create_node();
    else if (event->modifiers() == Qt::ControlModifier &&
             event->key() == Qt::Key_M)
        create_node();

    QWidget::keyPressEvent(event);
}
