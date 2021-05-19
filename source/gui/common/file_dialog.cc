#include <QApplication>
#include <QDateTime>
#include <QDesktopWidget>

#include <button.h>
#include <file_dialog.h>
#include <global.h>
#include <os.h>
#include <path_utils.h>

file_dialog::file_dialog(QWidget *parent)
    : QDialog(parent)
    , preview_image_visible(false)
    , history_current(0)
    , file_mode(true)
    , save_mode(false)
    , image_sequence(true)
    , current_dir("/home/pancho")
{
    layout = new QVBoxLayout(this);
    setObjectName("file_dialog");
    setMinimumSize({900, 500});

    // Widgets
    center_widget = new QWidget;
    preview_image = new QLabel;
    bottom_widget = new QWidget;
    //

    // Layouts
    QHBoxLayout *bottom_layout = new QHBoxLayout(bottom_widget);
    QHBoxLayout *center_layout = new QHBoxLayout(center_widget);
    //

    // Widgets new
    tree = new QTreeWidget;
    bookmark_tree = new QTreeWidget;
    path_edit = new QLineEdit;
    QLabel *filter_label = new QLabel("Filter:");
    filter_box = new combo_box();
    open_save_button = new QPushButton("Open");
    QPushButton *cancel_button = new QPushButton("Cancel");

    disk_path = new combo_box({{"Root: /", "/", false, ""},
                               {"Home: " + USER_DIR, USER_DIR, false, ""}});

    sequence_check = new knob_check_box({}, "Image Sequence", image_sequence);
    create_folder_name = new QLineEdit;
    splitter = new QSplitter;
    //

    // Tool Bar
    tool_bar = new tools();
    go_back_action = new action("Go Back Directory History", "", "arrow_left");
    go_forward_action =
        new action("Go Forward Directory History", "", "arrow_right");
    action *go_to_parent_action = new action("Go Back Parent", "", "arrow_up");
    image_preview_action = new action("Image Preview", "", "image");
    image_preview_action->set_checkable();
    action *create_directory_action = new action("Create Directory", "", "add");
    action *add_bookmark_action =
        new action("Add BookMark", "", "bookmark_add");
    action *remove_bookmark_action =
        new action("Remove BookMark", "", "bookmark_remove");
    //

    center_layout->setMargin(0);
    bottom_layout->setMargin(0);

    center_widget->setObjectName("center_widget");
    bottom_widget->setObjectName("bottom_widget");

    // Preview
    preview_image->setObjectName("preview_image");
    preview_image->setMinimumWidth(500);
    preview_image->setVisible(preview_image_visible);
    //

    create_folder_name->setPlaceholderText("Directory Name");

    // Splitter
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setObjectName("splitter");
    //

    // Tree
    tree->setSortingEnabled(true);
    tree->setObjectName("folder_tree");
    QStringList columns{"Name", "Size", "Type", "Data Modified"};
    tree->setHeaderLabels(columns);
    tree->setAlternatingRowColors(true);

    tree->setColumnWidth(0, 200);
    tree->setColumnWidth(1, 80);
    tree->setColumnWidth(2, 60);
    tree->setColumnWidth(3, 200);
    //

    // BookMark Tree
    bookmark_tree->setHeaderHidden(true);
    bookmark_tree->setAlternatingRowColors(true);

    bookmark_tree->setObjectName("bookmark_tree");
    //

    // Conecciones
    go_back_action->connect_to(this, [=]() {
        go_forward_action->set_disable(false);

        bool has_history = go_to_history(false);
        if (!has_history)
            go_back_action->set_disable(true);
    });

    go_forward_action->connect_to(this, [=]() {
        go_back_action->set_disable(false);

        bool has_history = go_to_history(true);
        if (!has_history)
            go_forward_action->set_disable(true);
    });

    create_directory_action->connect_to(this, [this]() { create_directory(); });

    go_to_parent_action->connect_to(this, [this]() { go_to_parent(); });

    image_preview_action->connect_to(this,
                                     [this]() { switch_preview_image(); });

    add_bookmark_action->connect_to(this, [this]() { add_bookmark(); });

    remove_bookmark_action->connect_to(this, [this]() { remove_bookmark(); });

    connect(open_save_button, &QPushButton::clicked, this,
            &file_dialog::open_or_save);

    connect(cancel_button, &QPushButton::clicked, this, [this] {
        files.clear();
        hide();
    });

    connect(sequence_check, &knob_check_box::changed, this,
            [this](bool checked) {
                image_sequence = checked;
                update();
            });

    connect(filter_box, &combo_box::changed, this,
            [this](QVariant value) { filter(value.toString()); });

    connect(disk_path, &combo_box::changed, this,
            [this](QVariant value) { enter_to_dir(value.toString()); });

    connect(tree, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem *item) { open_or_save(); });

    connect(tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) {
                current_filename = item->text(0);
                current_path = current_dir + "/" + item->text(0);
                set_path_edit(current_path, false);
                set_preview_image(item->text(0));
            });

    connect(bookmark_tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item) { open_bookmark(item->text(0)); });

    connect(path_edit, &QLineEdit::editingFinished, this,
            [this]() { enter_to_dir(path_edit->text()); });
    //

    // Layouts
    tool_bar->add_widget(disk_path);
    tool_bar->add_separator();
    tool_bar->add_action(add_bookmark_action);
    tool_bar->add_action(remove_bookmark_action);
    tool_bar->add_separator();
    tool_bar->add_action(image_preview_action);
    tool_bar->add_separator();
    tool_bar->add_widget(create_folder_name);
    tool_bar->add_action(create_directory_action);
    tool_bar->add_stretch();
    tool_bar->add_action(go_back_action);
    tool_bar->add_action(go_forward_action);
    tool_bar->add_action(go_to_parent_action);

    splitter->addWidget(bookmark_tree);
    splitter->addWidget(tree);

    center_layout->addWidget(splitter);
    center_layout->addWidget(preview_image);

    bottom_layout->addWidget(filter_label);
    bottom_layout->addWidget(filter_box);
    bottom_layout->addWidget(sequence_check);
    bottom_layout->addStretch();
    bottom_layout->addWidget(open_save_button);
    bottom_layout->addWidget(cancel_button);

    layout->addWidget(tool_bar);
    layout->addWidget(center_widget);
    layout->addWidget(path_edit);
    layout->addWidget(bottom_widget);

    // restaura los bookmark de usuario
    QJsonObject bookmarks_obj =
        jread(VINACOMP_CONF_PATH + "/file_dialog_bookmarks.json");

    for (QString dirname : bookmarks_obj.keys())
    {
        QString dirpath = bookmarks_obj.value(dirname).toString();
        add_bookmark(dirname, dirpath);
    }

    set_default();
    update();
}

file_dialog::~file_dialog() {}

int file_dialog::exec()
{
    update();
    splitter->setSizes({20, 140});
    create_folder_name->clear();
    center();

    history.clear();
    history_current = 0;
    add_history(current_dir);

    tree->sortByColumn(2, Qt::SortOrder::AscendingOrder);

    preview_image_visible = true;
    switch_preview_image();
    image_preview_action->set_checked(false);

    QDialog::exec();

    return files.count();
}

void file_dialog::set_default()
{
    set_file_mode();
    set_open_mode();
    set_image_sequence_option(false);
}

void file_dialog::update()
{
    set_path_edit(current_dir, true);

    for (QTreeWidgetItem *item : items)
        delete item;

    items.clear();
    tree->clear();

    current_path.clear();
    current_filename.clear();

    QStringList file_list = os::listdir(current_dir);
    if (image_sequence)
        file_list = filter_by_sequence(file_list);

    for (QString _path : file_list)
    {
        QString basename = os::basename(_path);

        QString ext = basename.split(".").last();
        if (!current_filters.empty() && !os::isdir(_path))
            if (!current_filters.contains(ext))
                continue;

        QTreeWidgetItem *item = new QTreeWidgetItem;

        QFileInfo file_info(_path);
        QString size = size_format(file_info.size());

        if (os::isdir(_path))
        {
            item->setIcon(0, QIcon("resources/images/folder_checked.png"));
            QFont font("Helvetica", 7, QFont::Bold);
            item->setFont(0, font);
            item->setFont(3, font);

            size = "...";
            ext = "...";
        }
        else if (basename.contains("#"))
        {
            item->setIcon(0, QIcon("resources/images/sequence_normal.png"));
            size = "...";
        }
        else
            item->setIcon(0, QIcon("resources/images/default_icon_normal.png"));

        item->setText(0, basename);
        item->setText(1, size);
        item->setText(2, ext.toUpper());
        item->setText(3, file_info.lastModified().toString());
        tree->addTopLevelItem(item);

        items.push_back(item);
    }

    if (save_mode && file_mode)
        open_save_button->setText("Save File");
    else if (save_mode && !file_mode)
        open_save_button->setText("Save Directory");
    else if (!save_mode && file_mode)
        open_save_button->setText("Open File");
    else if (!save_mode && !file_mode)
        open_save_button->setText("Open Folder");
}

void file_dialog::set_path_edit(QString _path, bool last_slash)
{
    if (_path.right(1) != '/' && last_slash)
        _path += '/';

    _path = _path.replace("//", "/");
    path_edit->setText(_path);
}

void file_dialog::go_to_parent()
{
    current_dir = os::dirname(current_dir);
    add_history(current_dir);
    update();
}

void file_dialog::enter_to_dir(QString dirpath)
{
    if (dirpath.isEmpty())
        return;

    dirpath = dirpath.replace("//", "/");

    if (!os::isdir(dirpath))
        return;

    current_dir = dirpath;
    add_history(current_dir);

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
    {
        dirpath = os::dirname(dirpath);
        dirname = os::basename(dirpath);
    }

    if (bookmarks.contains(dirname))
        return;

    add_bookmark(dirname, dirpath);
}

void file_dialog::add_bookmark(QString dirname, QString dirpath)
{
    if (dirname.isEmpty())
        return;

    QTreeWidgetItem *bookmark_item = new QTreeWidgetItem;
    bookmark_item->setIcon(0, QIcon("resources/images/bookmark_normal.png"));
    bookmark_item->setText(0, dirname);
    bookmark_tree->addTopLevelItem(bookmark_item);

    bookmarks.insert(dirname, {dirname, dirpath, bookmark_item});
}

void file_dialog::bookmark_backup()
{
    QJsonObject bookmarks_obj;
    for (auto bookmark : bookmarks)
        bookmarks_obj.insert(bookmark.name, bookmark.path);

    jwrite(VINACOMP_CONF_PATH + "/file_dialog_bookmarks.json", bookmarks_obj);
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

    add_history(current_dir);
    update();
}

void file_dialog::switch_preview_image()
{
    preview_image_visible = !preview_image_visible;

    if (preview_image_visible)
        this->setFixedWidth(1200);
    else
        this->setFixedWidth(900);

    preview_image->setVisible(preview_image_visible);

    center();
}

void file_dialog::center()
{
    QRect screenGeometry = qApp->desktop()->screenGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

QString file_dialog::size_format(int size)
{
    float num = size;
    QStringList list;
    list << "KB"
         << "MB"
         << "GB"
         << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while (num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 1) + " " + unit;
}

void file_dialog::set_preview_image(QString image_basename)
{
    if (!preview_image_visible)
        return;

    QStringList allowed_images = {"png", "jpg", "tiff"};

    QString ext = image_basename.split(".").last().toLower();
    if (!allowed_images.contains(ext))
    {
        preview_image->clear();
        return;
    }

    QString image_file = current_dir + "/" + image_basename;
    QPixmap pixmap(image_file);
    preview_image->setPixmap(pixmap.scaled(preview_image->width(), 1000,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
}

void file_dialog::open_or_save()
{
    if (save_mode && !os::isfile(current_path))
        current_path = path_edit->text();

    if (os::isdir(current_path))
    {
        enter_to_dir(current_dir + "/" + current_filename);
        return;
    }

    files.clear();
    files.push_back(current_path);
    hide();
}

void file_dialog::set_init_directory(QString directory)
{
    if (directory.isEmpty() || !os::isdir(directory))
        return;

    current_dir = directory;
    update();
}

void file_dialog::update_filter_box()
{
    for (auto filter : filters)
    {
        QString filters_string;
        for (QString ext : filter.filters)
            filters_string += "." + ext + "  ";

        filter_box->add_item(
            {filter.name + ":   " + filters_string, filter.name, false, ""});
    }

    filter_box->add_item({"All Files   *", "all_files", false, "multi_lines"});

    filter(filters.first().name);
}

void file_dialog::set_name_filter(QString name_filter, QStringList filters_list)
{
    filter_box->clear();
    filters.clear();

    filters.insert(name_filter, {name_filter, filters_list});
    update_filter_box();
}

void file_dialog::set_names_filters(QList<filter_struct> _filters)
{
    filter_box->clear();
    filters.clear();

    for (auto _filter : _filters)
        filters.insert(_filter.name, _filter);

    update_filter_box();
}

void file_dialog::filter(QString _filter)
{
    if (_filter == "all_files")
        current_filters = QStringList{};
    else
        current_filters = filters.value(_filter).filters;

    filter_box->set_value(_filter, false);

    update();
}

QStringList file_dialog::filter_by_sequence(QStringList files)
{
    QStringList temp_list;
    QStringList files_with_sequence;
    QMap<QString, int> padding_counts;

    for (QString _path : files)
    {
        QString path_no_padding = path_util::remove_padding(_path);

        if (temp_list.contains(path_no_padding))
        {
            // encuentra el padding mas larga de una misma secuencia
            int padding_count = path_util::get_padding(_path).length();
            if (!padding_counts.contains(path_no_padding))
                padding_counts.insert(path_no_padding, padding_count);
            else
            {
                int last_padding = padding_counts.value(path_no_padding);
                if (padding_count > last_padding)
                    padding_counts.insert(path_no_padding, padding_count);
            }
            //

            if (!files_with_sequence.contains(path_no_padding))
                files_with_sequence.push_back(path_no_padding);
        }
        else
            temp_list.push_back(path_no_padding);
    }

    QStringList final_list;

    for (QString _path : files)
    {
        QString path_no_padding = path_util::remove_padding(_path);

        if (files_with_sequence.contains(path_no_padding))
        {
            QString ext = path_util::get_ext(_path);
            QString padding = path_util::get_padding(_path);
            QString path_sequence = path_util::remove_ext(path_no_padding);

            QString sequence_sign;
            for (int i = 0; i < padding_counts.value(path_no_padding); i++)
                sequence_sign += "#";

            path_sequence += sequence_sign + "." + ext;

            if (!final_list.contains(path_sequence))
                final_list.push_back(path_sequence);
        }
        else
            final_list.push_back(_path);
    }

    return final_list;
}

void file_dialog::create_directory()
{
    QString name = create_folder_name->text();
    if (name.isEmpty())
        return;

    QString directory = current_dir + "/" + name;

    if (os::isdir(directory))
        return;

    os::makedir(directory);
    update();
}

void file_dialog::add_history(QString _path)
{
    if (_path.isEmpty())
        return;

    if (!history.empty())
        if (_path == history.first())
            return;

    history.push_front(_path);
    history_current = 0;

    go_back_action->set_disable(false);
    go_forward_action->set_disable(false);
}

bool file_dialog::go_to_history(bool forward)
{
    if (!forward)
    {
        if (history_current < history.count() - 1)
            history_current++;
        else
            return false;
    }
    else
    {
        if (history_current > 0)
            history_current--;
        else
            return false;
    }

    current_dir = history.value(history_current);
    update();

    return true;
}

void file_dialog::set_image_sequence(bool enable)
{
    image_sequence = enable;
}

void file_dialog::set_image_sequence_option(bool enable)
{
    sequence_check->set_visible(enable);
    set_image_sequence(enable);
}
