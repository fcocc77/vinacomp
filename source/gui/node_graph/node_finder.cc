#include <QHeaderView>
#include <QLabel>
#include <node_finder.h>
#include <qt.h>
#include <util.h>

node_finder::node_finder(QWidget *__node_view, nodes_load *_nodes)
    : QWidget(__node_view)
    , nodes(_nodes)
    , _node_view(__node_view)
    , size(QSize(300, 150))
{
    this->setObjectName("node_finder");
    this->setFixedSize(size);

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
        [=](QTreeWidgetItem *item) { search_field->setText(item->text(1)); });

    layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
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

        item->setText(1, node_name);
        item->setText(2, node_id);
        if (icon_name.contains("/"))
            item->setIcon(0, QIcon(icon_name));
        else
            item->setIcon(0, QIcon("resources/images/" + icon_name + "_normal.png"));

        QLabel *label = new QLabel(node_name, tree);
        tree->addTopLevelItem(item);
        tree->setItemWidget(item, 0, label);

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

QTreeWidgetItem *node_finder::get_item(QString key) const
{
    key = key.toUpper();

    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);
        if (item->text(1).toUpper().contains(key))
            return item;
    }

    return nullptr;
}

void node_finder::highlight_finded_char(QTreeWidgetItem *item, QString search_word)
{
    QLabel *label = static_cast<QLabel *>(tree->itemWidget(item, 0));

    QString node_name = item->text(1);

    int count = search_word.length();

    int index_left = node_name.toUpper().indexOf(search_word);
    int index_right = node_name.length() - index_left - count;

    QString finded_text = node_name.mid(index_left, count);

    QString highlight_text = node_name.left(index_left) +
                             "<font color=\"white\"><b>" + finded_text +
                             "</b></font>" + node_name.right(index_right);

    label->setText(highlight_text);
}

void node_finder::update_tree()
{
    QString text = search_field->text().toUpper();

    tree->clearSelection();


    int items_visible = 0;

    for (int i = 0; i < tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = tree->topLevelItem(i);

        bool hidden = false;
        if (!item->text(1).toUpper().contains(text))
            hidden = true;

        if (text.isEmpty())
            hidden = false;

        item->setHidden(hidden);

        if (!hidden)
        {
            items_visible++;
            highlight_finded_char(item, text);
        }
    }
    
    int item_height = 21;
    int items_height = items_visible * item_height;

    if (items_height < size.height())
        tree->setFixedHeight(items_height);
    else
        tree->setFixedHeight(QWIDGETSIZE_MAX);
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

    int mid_width = size.width() / 2;

    int pos_x = position.x() - mid_width;
    int pos_y = position.y();

    if (pos_y < 0)
        pos_y = 0;

    if (position.x() < mid_width)
        pos_x = 0;

    if ((position.x() + mid_width) > _node_view->width())
        pos_x = _node_view->width() - size.width();

    bool reverse = (position.y() + size.height()) > _node_view->height();
    if (reverse)
    {
        layout->setDirection(QBoxLayout::BottomToTop);
        layout->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
        this->move(pos_x, pos_y - size.height());
    }
    else
    {
        layout->setDirection(QBoxLayout::TopToBottom);
        layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
        this->move(pos_x, pos_y);
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

    QString node_id = item->text(2);
    QString node_name = item->text(1);

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
