#include <panels_layout.hpp>

panels_layout::panels_layout(
    node_graph *_node_graph,
    viewer *_viewer,
    script_editor *_script_editor,
    properties *_properties,
    curve_editor *_curve_editor)
{

    this->setObjectName("panels_layout");

    splitters = new QList<QSplitter *>;

    first_panel = new panel(this, splitters, _node_graph, _viewer, _script_editor, _properties, _curve_editor);

    QWidget *central_widget = new QWidget();
    qt::add_widget(this, first_panel);

    restore_default_action = new QAction("Restore Default Layout");
    save_current_action = new QAction("Save Current Layout");

    connect(save_current_action, &QAction::triggered, this, &panels_layout::save_layout);
    connect(restore_default_action, &QAction::triggered, this, &panels_layout::restore_default);

    load_layout();
}

panels_layout::~panels_layout()
{
}

panel *panels_layout::get_child_panel(QSplitter *splitter, QString _letter)
{
    // Obtiene el widget hijo del splitter 'a' o 'b'
    QWidget *container;
    if (_letter == "a")
        container = splitter->widget(0);
    else
        container = splitter->widget(1);

    return container->findChild<panel *>();
}

void panels_layout::save_json_layout(QSplitter *splitter, int deep, QString letter, QStringList parents)
{

    if (splitter != NULL)
    {
        if (!letter.isEmpty())
        {
            panel *child_panel = get_child_panel(splitter, letter);

            if (child_panel->objectName() == "panel")
                return;
            else
            {
                for (QSplitter *_splitter : *splitters)
                {
                    if (_splitter->objectName() == child_panel->objectName())
                    {
                        splitter = _splitter;
                        break;
                    }
                }
            }
        }

        deep++;

        QString parent_name;
        if (deep == 1)
            parent_name = "splitter";
        else
            parent_name = "splitter_" + letter;
        parents.push_back(parent_name);

        int orientation = splitter->orientation();
        auto sizes = splitter->sizes();
        //
        //

        QJsonObject value;

        value["orientation"] = orientation;
        value["distribution"] = QJsonArray{sizes[0], sizes[1]};

        // Crea los objetos a y b de un splitter y si el hijo es un panel,
        // guarda los datos de los 'tabs' del panel.
        panel *child_panel_a = get_child_panel(splitter, "a");
        panel *child_panel_b = get_child_panel(splitter, "b");

        QJsonObject splitter_a;
        QJsonObject splitter_b;
        if (child_panel_a->objectName() == "panel")
            value["panel_a"] = qt::list_to_array(child_panel_a->tabs_list);
        else
            value["splitter_a"] = {};

        if (child_panel_b->objectName() == "panel")
            value["panel_b"] = qt::list_to_array(child_panel_b->tabs_list);
        else
            value["splitter_b"] = {};
        //
        //

        qt::insert_json_deep(&json_layout, parents, value);

        save_json_layout(splitter, deep, "a", parents);
        save_json_layout(splitter, deep, "b", parents);
    }
}

void panels_layout::save_layout()
{
    QSplitter *main_splitter = this->findChild<QSplitter *>("splitter");

    // vaciar layout json
    json_layout = {};
    //

    save_json_layout(main_splitter);
    jwrite("temp/layout.json", json_layout);
}

void panels_layout::load_splitter(QJsonObject splitter_obj, panel *panel_a)
{
    QJsonArray distribution = splitter_obj["distribution"].toArray();
    int distribution_a = distribution[0].toInt();
    int distribution_b = distribution[1].toInt();

    int orientation = splitter_obj["orientation"].toInt();

    panel *panel_b = panel_a->split(Qt::Orientation(orientation));
    panel_b->get_splitter()->setSizes({distribution_a, distribution_b});

    QJsonObject splitter_a = splitter_obj["splitter_a"].toObject();
    QJsonObject splitter_b = splitter_obj["splitter_b"].toObject();

    QJsonArray panel_a_obj = splitter_obj["panel_a"].toArray();
    QJsonArray panel_b_obj = splitter_obj["panel_b"].toArray();

    if (!splitter_a.isEmpty())
        load_splitter(splitter_a, panel_a);
    if (!splitter_b.isEmpty())
        load_splitter(splitter_b, panel_b);

    if (!panel_a_obj.isEmpty())
        panel_a->add_tabs(qt::array_to_list(panel_a_obj));
    if (!panel_b_obj.isEmpty())
        panel_b->add_tabs(qt::array_to_list(panel_b_obj));
}

void panels_layout::load_layout()
{
    QJsonObject layout = jread("temp/layout.json");
    QJsonObject main = layout["splitter"].toObject();

    load_splitter(main, first_panel);
}

void panels_layout::restore_default()
{
    QList<panel *> panels = this->findChildren<panel *>("panel");
    for (panel *_panel : panels)
    {
        _panel->remove_all_tab();
        _panel->close_panel();
    }

    QJsonObject layout = jread("resources/data/default_layout.json");
    QJsonObject main = layout["splitter"].toObject();

    load_splitter(main, first_panel);
}