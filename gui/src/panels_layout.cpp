#include <panels_layout.hpp>

panels_layout::panels_layout(/* args */)
{
    this->setObjectName("panels_layout");

    splitters = new QList<QSplitter *>;

    panel *_panel = new panel(splitters);

    QWidget *central_widget = new QWidget();
    qt::add_widget(this, _panel);

    restore_default_action = new QAction("Restore Default Layout");
    save_current_action = new QAction("Save Current Layout");

    connect(save_current_action, &QAction::triggered, this, &panels_layout::save_layout);
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

void panels_layout::load_layout()
{
}

void panels_layout::restore_default()
{
}