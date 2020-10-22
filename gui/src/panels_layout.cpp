#include <panels_layout.hpp>

panels_layout::panels_layout(/* args */)
{
    this->setObjectName("panels_layout");

    splitters = new QList<QSplitter *>;

    first_panel = new panel(splitters);

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

void panels_layout::update_json_layout(QSplitter *splitter, int deep, QString letter, QStringList parents)
{
    if (splitter != NULL)
    {
        if (!letter.isEmpty())
        {
            QWidget *container;
            if (letter == "a")
                container = splitter->widget(0);
            else
                container = splitter->widget(1);

            QWidget *_panel = container->findChild<panel *>();

            // si no es un panel es un QSplitter
            if (_panel->objectName() != "panel")
            {
                for (QSplitter *_splitter : *splitters)
                {
                    if (_splitter->objectName() == _panel->objectName())
                    {
                        splitter = _splitter;
                        break;
                    }
                }
            }
            else
            {
                return;
            }
        }

        deep++;

        QString parent_name;
        if (deep == 1)
            parent_name = "splitter";
        else
            parent_name = "splitter_" + letter;

        int orientation = splitter->orientation();
        auto sizes = splitter->sizes();

        QJsonObject splitter_json = {{{"orientation", orientation},
                                      {"distribution", QJsonArray{sizes[0], sizes[1]}},
                                      {"splitter_a", QJsonObject()},
                                      {"splitter_b", QJsonObject()}}};

        parents.push_back(parent_name);
        qt::insert_json_deep(&json_layout, parents, splitter_json);

        update_json_layout(splitter, deep, "a", parents);
        update_json_layout(splitter, deep, "b", parents);
    }
}

void panels_layout::save_layout()
{
    QSplitter *main_splitter = this->findChild<QSplitter *>("splitter");

    // vaciar layout json
    json_layout = {};
    //

    update_json_layout(main_splitter);
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

    if (!splitter_a.isEmpty())
        load_splitter(splitter_a, panel_a);
    if (!splitter_b.isEmpty())
        load_splitter(splitter_b, panel_b);
}

void panels_layout::load_layout()
{
    QJsonObject layout = jread("temp/layout.json");
    QJsonObject main = layout["splitter"].toObject();

    load_splitter(main, first_panel);
}

void panels_layout::restore_default()
{
}