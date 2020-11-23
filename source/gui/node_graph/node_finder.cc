#include <node_finder.h>

node_finder::node_finder(QWidget *__node_graph, nodes_load *_nodes)
    : _node_graph(__node_graph),
      nodes(_nodes)
{

    this->setParent(_node_graph);
    search_field = new QLineEdit(this);
    connect(search_field, &QLineEdit::textChanged, this, &node_finder::update_tree);

    tree = new QTreeWidget(this);

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    layout->addWidget(search_field);
    layout->addWidget(tree);

    for (QJsonValue value : nodes->get_effects())
    {
        QString node_name = value.toObject()["label"].toString();
        QString node_id = value.toObject()["id"].toString();
        QTreeWidgetItem *item = new QTreeWidgetItem();

        item->setText(1, node_id);
        item->setText(0, node_name);
        tree->addTopLevelItem(item);
    }

    this->hide();
}

node_finder::~node_finder()
{
}

void node_finder::update_tree()
{

    QString text = search_field->text().toUpper();

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
}

void node_finder::set_focus()
{
    search_field->setFocus();
}

void node_finder::clear()
{
    search_field->setText("");
}

void node_finder::show_finder()
{
    this->clear();
    this->set_focus();
    QPoint position = _node_graph->mapFromGlobal(QCursor::pos());

    int mid_width = 130;

    this->move(position.x() - mid_width, position.y());
    this->show();
}