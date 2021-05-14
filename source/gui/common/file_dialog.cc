#include <file_dialog.h>
#include <os.h>
#include <button.h>

file_dialog::file_dialog(QWidget *parent)
    : QDialog(parent)
    , current_dir("/home/pancho")
{
    layout = new QVBoxLayout(this);
    setObjectName("file_dialog");
    setMinimumSize({700, 500});

    // Tool Bar
    tool_bar = new tools();
    action *go_back_action = new action("Go Back", "", "arrow_left");
    action *go_forward_action = new action("Go Forward", "", "arrow_right");
    action *go_to_parent_action = new action("Go Back", "", "arrow_up");
    go_to_parent_action->connect_to(this, [this]() { go_to_parent(); });
    action *create_folder =
        new action("Create Directorio", "", "create_new_folder");

    tool_bar->add_stretch();
    tool_bar->add_action(go_back_action);
    tool_bar->add_action(go_forward_action);
    tool_bar->add_action(go_to_parent_action);
    tool_bar->add_action(create_folder);
    //

    // Widgets
    center_widget = new QWidget;
    bookmark_widget = new QWidget;
    preview_widget = new QWidget;
    bottom_widget = new QWidget;
    directory_widget = new QWidget;
    QWidget *bookmark_buttons = new QWidget;
    //

    // Layouts
    QHBoxLayout *bottom_layout = new QHBoxLayout(bottom_widget);
    QVBoxLayout *bookmark_widget_layout = new QVBoxLayout(bookmark_widget);
    QHBoxLayout *bookmark_buttons_layout = new QHBoxLayout(bookmark_buttons);
    QHBoxLayout *center_layout = new QHBoxLayout(center_widget);
    QVBoxLayout *directory_layout = new QVBoxLayout(directory_widget);
    //

    // Widgets new
    tree = new QTreeWidget;
    bookmark_tree = new QTreeWidget;
    button *add_bookmark_button = new button();
    button *remove_bookmark_button = new button();
    path_edit = new QLineEdit;
    QLabel *filter_label = new QLabel("Filter:");
    QLineEdit *filter_line = new QLineEdit;
    QPushButton *open_save_button = new QPushButton("Open");
    QPushButton *cancel_button = new QPushButton("Cancel");
    //

    center_widget->setObjectName("center_widget");
    center_layout->setMargin(0);


    directory_layout->setMargin(0);
    connect(tree, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem *item) { enter_to_dir(item->text(0)); });

    connect(bookmark_tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { open_bookmark(item->text(0)); });

    bottom_widget->setObjectName("bottom_widget");
    bottom_layout->setMargin(0);
    connect(cancel_button, &QPushButton::clicked, this, [this] { hide(); });

    // BookMark buttons
    bookmark_widget->setObjectName("bookmark_widget");
    bookmark_buttons->setObjectName("bookmark_buttons");
    add_bookmark_button->set_icon("bookmark_add");
    remove_bookmark_button->set_icon("bookmark_remove");
    connect(add_bookmark_button, &button::clicked, this, &file_dialog::add_bookmark);
    connect(remove_bookmark_button, &button::clicked, this, &file_dialog::remove_bookmark);
    //

    // Layouts Construccion
    bookmark_buttons_layout->addStretch();
    bookmark_buttons_layout->addWidget(remove_bookmark_button);
    bookmark_buttons_layout->addWidget(add_bookmark_button);

    bookmark_widget_layout->addWidget(bookmark_tree);
    bookmark_widget_layout->addWidget(bookmark_buttons);

    directory_layout->addWidget(tree);
    directory_layout->addWidget(path_edit);

    center_layout->addWidget(bookmark_widget);
    center_layout->addWidget(directory_widget);
    center_layout->addWidget(preview_widget);

    bottom_layout->addWidget(filter_label);
    bottom_layout->addWidget(filter_line);
    bottom_layout->addWidget(open_save_button);
    bottom_layout->addWidget(cancel_button);

    layout->addWidget(tool_bar);
    layout->addWidget(center_widget);
    layout->addWidget(bottom_widget);

    update();
}

file_dialog::~file_dialog() {}

void file_dialog::update()
{
    path_edit->setText(current_dir);

    for (QTreeWidgetItem *item : items)
        delete item;

    items.clear();
    tree->clear();

    for (QString _path : os::listdir(current_dir))
    {
        QString basename = os::basename(_path);
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, basename);
        tree->addTopLevelItem(item);

        items.push_back(item);
    }
}
void file_dialog::go_to_parent()
{
    current_dir = os::dirname(current_dir);
    update();
}

void file_dialog::enter_to_dir(QString dirname)
{
    QString dirpath = current_dir + "/" + dirname;
    dirpath = dirpath.replace("//", "/");
    if (!os::isdir(dirpath))
        return;

    current_dir = dirpath;
    update();
}

void file_dialog::add_bookmark()
{
    QString dirname, dirpath;

    if (!tree->selectedItems().empty())
    {
        auto *selected_item = tree->selectedItems().first();
        dirname = selected_item->text(0);
        dirpath = current_dir + "/" + dirname;
    }
    else
    {
        dirpath = path_edit->text();
        dirname = os::basename(dirpath);
    }

    dirpath = dirpath.replace("//", "/");

    if (!os::isdir(dirpath))
        return;

    if (bookmarks.contains(dirname))
        return;

    QTreeWidgetItem *bookmark_item = new QTreeWidgetItem;
    bookmark_item->setText(0, dirname);
    bookmark_tree->addTopLevelItem(bookmark_item);

    bookmarks.insert(dirname, {dirname, dirpath, bookmark_item});
}

void file_dialog::remove_bookmark()
{
    if (bookmark_tree->selectedItems().empty())
        return;

    auto *selected_item = bookmark_tree->selectedItems().first();
    QString bookmark = selected_item->text(0);

    delete bookmarks.value(bookmark).item;
    bookmarks.remove(bookmark);

    bookmark_tree->removeItemWidget(selected_item, 0);
}

void file_dialog::open_bookmark(QString _bookmark)
{
    bookmark mark = bookmarks.value(_bookmark);
    current_dir = mark.path;
    update();
}
