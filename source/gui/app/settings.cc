#include <settings.h>
#include <vinacomp.h>

settings::settings(bool has_dialog_buttons, QWidget *__vinacomp)
    : _vinacomp(__vinacomp)
    , knobs(new QMap<QString, knob *>)
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

    QWidget *right_widget = new QWidget;
    QVBoxLayout *right_layout = new QVBoxLayout(right_widget);
    right_layout->setMargin(0);
    right_layout->setSpacing(0);

    right_layout->addWidget(content);

    if (has_dialog_buttons)
    {
        QWidget *dialog_buttons = new QWidget;
        dialog_buttons->setObjectName("dialog_buttons");
        dialog_buttons->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Fixed);
        QHBoxLayout *dialog_buttons_layout = new QHBoxLayout(dialog_buttons);

        QPushButton *save_button = new QPushButton("Save Settings");
        QPushButton *cancel_button = new QPushButton("Cancel");

        connect(save_button, &QPushButton::clicked, this, [this]() {
            static_cast<vinacomp *>(_vinacomp)->settings_action->set_checked(
                false, true);
            save_settings();
        });

        connect(cancel_button, &QPushButton::clicked, this, [this]() {
            static_cast<vinacomp *>(_vinacomp)->settings_action->set_checked(
                false, true);
        });

        dialog_buttons_layout->addStretch();
        dialog_buttons_layout->addWidget(save_button);
        dialog_buttons_layout->addWidget(cancel_button);

        right_layout->addWidget(dialog_buttons);
    }

    splitter->addWidget(tree);
    splitter->addWidget(right_widget);
    splitter->setSizes({100, 200});

    layout->addWidget(splitter);
}

settings::~settings()
{
    delete knobs;
}

knob *settings::get_knob(QString name) const
{
    return knobs->value(name);
}

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

void settings::save_settings() {}
