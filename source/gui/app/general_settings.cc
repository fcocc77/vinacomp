#include <QSplitter>

#include <button.h>
#include <general_settings.h>
#include <global.h>
#include <knob_check_box.h>
#include <knob_integer.h>
#include <os.h>
#include <path_utils.h>
#include <setup_knobs.h>
#include <util.h>

general_settings::general_settings(QWidget *_vinacomp)
    : settings(true, _vinacomp)
    , data(new QJsonObject)
{
    setup_general();
    setup_auto_save();
    setup_plugins();
    setup_appearance();

    set_content("AutoSave");
}

general_settings::~general_settings()
{
    delete data;
}

void general_settings::setup_general()
{
    QVBoxLayout *layout = add_item("General");
}

void general_settings::setup_auto_save()
{
    QVBoxLayout *layout = add_item("AutoSave");

    setup_knobs_props props;

    QJsonObject enable_knob = {{"type", "check_box"},
                               {"name", "auto_save_enable"},
                               {"label", "Enable AutoSave"},
                               {"default", true}};

    QJsonObject delay_knob = {{"type", "integer"},
                              {"name", "auto_save_delay"},
                              {"label", "Delay: Seconds"},
                              {"min", 1},
                              {"max", 30},
                              {"default", 5}};

    props.knobs_array.push_back(enable_knob);
    props.knobs_array.push_back(delay_knob);

    props.layout = layout;
    props.knobs = knobs;
    props.params = data;

    setup_knobs(props);
}

void general_settings::setup_plugins()
{
    QVBoxLayout *layout = add_item("Plugins");

    plugin_tree = new QTreeWidget;
    plugin_tree->setObjectName("plugin_tree");
    plugin_tree->setAlternatingRowColors(true);
    QStringList columns{"Plugin Name", "Label", "Group"};
    plugin_tree->setHeaderLabels(columns);
    plugin_tree->setColumnWidth(0, 100);
    plugin_tree->setColumnWidth(1, 100);
    plugin_tree->setColumnWidth(2, 100);

    QWidget *buttons = new QWidget;
    buttons->setObjectName("plugin_buttons");
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons);
    buttons_layout->setMargin(0);

    button *remove_button = new button();
    button *edit_button = new button();

    remove_button->set_icon("delete");
    edit_button->set_icon("edit");

    buttons_layout->addWidget(edit_button);
    buttons_layout->addWidget(remove_button);
    buttons_layout->addStretch();

    layout->addWidget(plugin_tree);
    layout->addWidget(buttons);

    load_plugins();
}

void general_settings::load_plugins()
{
    QString base_path = "plugins/py_plugins/";
    for (QString plugin_path : os::listdir(base_path))
    {
        if (path_util::get_ext(plugin_path) != "json")
            continue;

        QJsonObject plugin = jread(plugin_path);

        QString name = plugin.value("id").toString();
        QString icon = plugin.value("icon").toString();
        QString label = plugin.value("label").toString();
        QString group = plugin.value("group").toString();

        QString icon_group = base_path + "/" + group + ".png";

        QTreeWidgetItem *item = new QTreeWidgetItem;

        item->setText(0, name);
        item->setText(1, label);
        item->setText(2, group);

        item->setIcon(0, QIcon(icon));

        if (os::isfile(icon_group))
            item->setIcon(2, QIcon(icon_group));

        plugin_tree->addTopLevelItem(item);
    }
}

void general_settings::setup_appearance()
{
    QVBoxLayout *layout = add_item("Appearance");
}

void general_settings::save_settings()
{
    jwrite(VINACOMP_CONF_PATH + "/settings.json", *data);
}

void general_settings::showEvent(QShowEvent *event)
{
    restore_settings();
}

QWidget *get_knob(QString name);

void general_settings::restore_settings()
{
    *data = jread(VINACOMP_CONF_PATH + "/settings.json");

    for (knob *_knob : *knobs)
        _knob->restore_param();
}
