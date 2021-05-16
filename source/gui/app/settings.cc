#include <settings.h>

settings::settings()
{
    this->hide();
    this->setObjectName("settings");
    this->setFixedWidth(500);

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    tree = new QTreeWidget();
    tree->setHeaderHidden(true);
    tree->setAlternatingRowColors(true);

    connect(tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { set_content(item->text(0)); });

    QSplitter *splitter = new QSplitter;
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *content = new QWidget;
    content_layout = new QHBoxLayout(content);
    content_layout->setMargin(0);
    content->setObjectName("settings_content");

    splitter->addWidget(tree);
    splitter->addWidget(content);
    splitter->setSizes({100, 200});

    layout->addWidget(splitter);
}

settings::~settings() {}

QVBoxLayout *settings::add_item(QString name)
{
    QWidget *widget = new QWidget;
    widget->hide();
    widget->setObjectName("settings_content");

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    content_layout->addWidget(widget);

    QTreeWidgetItem *tree_item = new QTreeWidgetItem;
    tree_item->setText(0, name);
    tree->addTopLevelItem(tree_item);

    items.push_back({name, widget, tree_item});

    return layout;
}

void settings::set_content(QString content_name)
{
    QTreeWidgetItem *_item = 0;

    for (item_struct item : items)
    {
        item.widget->setVisible(content_name == item.name);
        if (content_name == item.name)
            _item = item.item;
    }

    if (_item)
        tree->setItemSelected(_item, true);
}
