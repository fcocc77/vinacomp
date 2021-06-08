#include <general_settings.h>
#include <nodes_load.h>
#include <os.h>
#include <path_utils.h>
#include <vinacomp.h>
#include <global.h>
#include <tools.h>
#include <path_utils.h>

void general_settings::setup_plugins()
{
    QVBoxLayout *layout = add_item("Plugins");

    plugin_dirs_tree = new QTreeWidget;
    plugin_dirs_tree->setObjectName("plugin_tree");
    plugin_dirs_tree->setAlternatingRowColors(true);
    QStringList plugin_dirs_columns{"Dir Name", "Path", "Plugins Amount"};
    plugin_dirs_tree->setHeaderLabels(plugin_dirs_columns);
    plugin_dirs_tree->setColumnWidth(0, 100);
    plugin_dirs_tree->setColumnWidth(1, 100);
    plugin_dirs_tree->setColumnWidth(2, 100);

    plugin_tree = new QTreeWidget;
    plugin_tree->setObjectName("plugin_tree");
    plugin_tree->setAlternatingRowColors(true);
    QStringList columns{"Plugin Name", "Label", "Group"};
    plugin_tree->setHeaderLabels(columns);
    plugin_tree->setColumnWidth(0, 100);
    plugin_tree->setColumnWidth(1, 100);
    plugin_tree->setColumnWidth(2, 100);

    tools *plugin_dirs_tools = new tools;
    tools *plugin_tools = new tools;

    plugin_dirs_tools->get_layout()->setMargin(0);
    plugin_tools->get_layout()->setMargin(0);

    action *remove_dir_action = new action("Remove Dir", "", "delete");
    action *add_dir_action =
        new action("Add Plugin Dir", "", "create_new_folder");
    action *reload_plugin_action =
        new action("Reload Plugin Dirs", "", "refresh");

    action *remove_action = new action("Remove Plugin", "", "delete");
    action *edit_action = new action("Edit Plugin", "", "edit");

    // Conecciones
    remove_action->connect_to(this, [=]() { delete_plugin(); });
    add_dir_action->connect_to(this, [=]() { add_plugin_dir_dialog(); });

    reload_plugin_action->connect_to(this,
                                     [=]() { reload_plugin_dirs(); });
    remove_dir_action->connect_to(this, [=](){delete_plugin_dir();});

    // Layout
    plugin_dirs_tools->add_action(add_dir_action);
    plugin_dirs_tools->add_action(remove_dir_action);
    plugin_dirs_tools->add_action(reload_plugin_action);

    plugin_tools->add_action(remove_action);
    plugin_tools->add_action(edit_action);

    layout->addWidget(plugin_dirs_tree);
    layout->addWidget(plugin_dirs_tools);
    layout->addWidget(plugin_tree);
    layout->addWidget(plugin_tools);

    load_plugins();
}

void general_settings::load_plugins()
{
    QString base_path = PY_PLUGINS_PATH + "/";
    for (QString plugin_path : os::listdir(base_path))
    {
        if (path_util::get_ext(plugin_path) != "json")
            continue;

        QJsonObject plugin = jread(plugin_path);

        QString name = plugin.value("id").toString();
        QString icon = plugin.value("icon").toString();
        QString label = plugin.value("label").toString();
        QString group = plugin.value("group").toString();


        QTreeWidgetItem *item = new QTreeWidgetItem;

        item->setText(0, name);
        item->setText(1, label);
        item->setText(2, group);

        QString default_icon = "resources/images/default_icon_normal.png";

        if (!os::isfile(icon))
            icon = default_icon;

        item->setIcon(0, QIcon(icon));

        QString icon_group = base_path + "/" + group + ".png";

        if (!os::isfile(icon_group))
            icon_group = default_icon;

        item->setIcon(2, QIcon(icon_group));

        plugin_tree->addTopLevelItem(item);
        plugin_items.push_back(item);
    }
}

void general_settings::update_plugin_tree()
{
    for (auto *item : plugin_items)
        delete item;

    plugin_items.clear();
    plugin_tree->clear();

    load_plugins();
}

void general_settings::update_plugin_dirs_tree()
{
    for (auto *item : plugin_dirs_items)
        delete item;

    plugin_dirs_items.clear();
    plugin_dirs_tree->clear();

    QJsonObject plugin_dirs_list = data->value("plugin_dirs").toObject();

    for (QJsonValue value: plugin_dirs_list)
    {
        QString dir = value.toString();
        QString name = os::basename(dir);
        int plugin_amount = 0;

        QTreeWidgetItem *item = new QTreeWidgetItem;

        item->setText(0, name);
        item->setText(1, dir);
        item->setText(2, QString::number(plugin_amount));

        item->setIcon(0, QIcon("resources/images/reload_plugin_normal.png"));

        plugin_dirs_tree->addTopLevelItem(item);
        plugin_dirs_items.push_back(item);
    }
}

void general_settings::delete_plugin_dir()
{
    if (plugin_dirs_tree->selectedItems().isEmpty())
        return;

    auto *item = plugin_dirs_tree->selectedItems().first();
    QString plugin_dir_name = item->text(0);

    QJsonObject plugin_dirs_list = data->value("plugin_dirs").toObject();
    plugin_dirs_list.remove(plugin_dir_name);

    data->insert("plugin_dirs", plugin_dirs_list);
    update_plugin_dirs_tree();
}

void general_settings::delete_plugin()
{
    if (plugin_tree->selectedItems().isEmpty())
        return;

    auto *item = plugin_tree->selectedItems().first();

    QString plugin_name = item->text(0);
    QString base_plugin_path = PY_PLUGINS_PATH + "/" + plugin_name;

    os::remove(base_plugin_path + ".json");
    os::remove(base_plugin_path + ".py");
    os::remove(base_plugin_path + ".png");

    nodes_load *nodes_loaded = static_cast<vinacomp *>(_vinacomp)
                                   ->get_main_node_graph()
                                   ->get_nodes_loaded();

    update_plugin_tree();
    nodes_loaded->update_py_plugins();
}

void general_settings::reload_plugin_dirs()
{
    QJsonObject plugin_dirs_list = data->value("plugin_dirs").toObject();
    for (QJsonValue value : plugin_dirs_list)
        reload_plugin_dir(value.toString());
}

void general_settings::reload_plugin_dir(QString plugin_dir)
{
    // lista de los '.py' que contengan la palabra 'VinaComp Plugin'
    QStringList py_list;
    for (QString file : os::listdir(plugin_dir, true))
        if (path_util::get_ext(file) == "py")
            if (read_first_line(file).contains("VinaComp Plugin"))
                py_list.push_back(file);
    //

    script_editor *_script_editor =
        static_cast<vinacomp *>(_vinacomp)->get_script_editor();

    for (QString py_file : py_list)
        _script_editor->load_module(py_file);
}

void general_settings::add_plugin_dir_dialog()
{
    file_dialog *dialog = static_cast<vinacomp *>(_vinacomp)->get_file_dialog();
    dialog->set_dir_mode();

    if (!dialog->exec())
        return;

    QString dir = dialog->get_files().first();
    QString name = os::basename(dir);

    QJsonObject plugin_dirs_list = data->value("plugin_dirs").toObject();
    plugin_dirs_list.insert(name, dir);

    data->insert("plugin_dirs", plugin_dirs_list);
    update_plugin_dirs_tree();
}
