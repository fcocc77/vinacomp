#include <QSplitter>

#include <settings.h>
#include <knob_integer.h>
#include <knob_check_box.h>

settings::settings()
    : general_widget(0)
    , auto_save_widget(0)
    , plugins_widget(0)
    , appearance_widget(0)
{
    this->hide();
    this->setObjectName("settings");
    this->setFixedWidth(500);

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QTreeWidget *tree = new QTreeWidget();
    tree->setHeaderHidden(true);
    tree->setAlternatingRowColors(true);

    connect(tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { set_content(item->text(0)); });

    QTreeWidgetItem *general_item = new QTreeWidgetItem;
    QTreeWidgetItem *auto_save_item = new QTreeWidgetItem;
    QTreeWidgetItem *plugins_item = new QTreeWidgetItem;
    QTreeWidgetItem *appearance_item = new QTreeWidgetItem;

    general_item->setText(0, "General");
    auto_save_item->setText(0, "AutoSave");
    plugins_item->setText(0, "Plugins");
    appearance_item->setText(0, "Appearance");

    tree->addTopLevelItem(general_item);
    tree->addTopLevelItem(auto_save_item);
    tree->addTopLevelItem(plugins_item);
    tree->addTopLevelItem(appearance_item);

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

    set_content("General");
}

settings::~settings() {}

void settings::set_content(QString content_name)
{
    set_visible_general(content_name == "General");
    set_visible_auto_save(content_name == "AutoSave");
    set_visible_plugins(content_name == "Plugins");
    set_visible_appearance(content_name == "Appearance");
}

void settings::set_visible_general(bool visible)
{
    if (general_widget)
    {
        general_widget->setVisible(visible);
        return;
    }

    general_widget = new QWidget;
    general_widget->setObjectName("general_widget");
    QVBoxLayout *layout = new QVBoxLayout(general_widget);

    content_layout->addWidget(general_widget);
    general_widget->setVisible(visible);
}

void settings::set_visible_auto_save(bool visible)
{
    if (auto_save_widget)
    {
        auto_save_widget->setVisible(visible);
        return;
    }

    auto_save_widget = new QWidget;
    auto_save_widget->setObjectName("auto_save_widget");
    QVBoxLayout *layout = new QVBoxLayout(auto_save_widget);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    int init_space = 100;

    knob_check_box *enable_knob =
        new knob_check_box({}, "Enable AutoSave", true);
    knob_integer *delay_knob = new knob_integer({}, 1, 30, 5);

    layout->addWidget(enable_knob);
    layout->addWidget(delay_knob);

    enable_knob->set_init_space(init_space);
    delay_knob->set_init_space(init_space);
    delay_knob->set_init_label_text("Delay: Seconds");

    content_layout->addWidget(auto_save_widget);
    auto_save_widget->setVisible(visible);
}

void settings::set_visible_plugins(bool visible)
{
    if (plugins_widget)
    {
        plugins_widget->setVisible(visible);
        return;
    }

    plugins_widget = new QWidget;
    plugins_widget->setObjectName("plugins_widget");
    QVBoxLayout *layout = new QVBoxLayout(plugins_widget);

    content_layout->addWidget(plugins_widget);
    plugins_widget->setVisible(visible);
}

void settings::set_visible_appearance(bool visible)
{
    if (appearance_widget)
    {
        appearance_widget->setVisible(visible);
        return;
    }

    appearance_widget = new QWidget;
    appearance_widget->setObjectName("appearance_widget");
    QVBoxLayout *layout = new QVBoxLayout(appearance_widget);

    content_layout->addWidget(appearance_widget);
    appearance_widget->setVisible(visible);
}
