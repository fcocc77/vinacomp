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

void panels_layout::add_json(QSplitter *_split, int deep, QString letter)
{

    if (_split != NULL)
    {

        QWidget *container_a = _split->widget(0);
        QWidget *container_b = _split->widget(1);

        QWidget *widget_a = container_a->findChild<panel *>();
        QWidget *widget_b = container_b->findChild<panel *>();

        deep++;

        QString parent_name;
        if (deep == 1)
            parent_name = "widget";
        else
            parent_name = "widget_" + letter + "_" + QString::number(deep - 1);

        QString splitter_name_a = "widget_a_" + QString::number(deep);
        QString splitter_name_b = "widget_b_" + QString::number(deep);

        json_layout[parent_name] = {{{splitter_name_a, splitter_name_a},
                                     {splitter_name_b, splitter_name_b}}};

        //
        //

        // si no es un panel es un QSplitter
        if (widget_a->objectName() != "panel")
        {
            for (QSplitter *splitter : *splitters)
            {
                if (splitter->objectName() == widget_a->objectName())
                {

                    add_json(splitter, deep, "a");
                    break;
                }
            }
        }

        if (widget_b->objectName() != "panel")
        {
            for (QSplitter *splitter : *splitters)
            {
                if (splitter->objectName() == widget_b->objectName())
                {
                    add_json(splitter, deep, "b");
                    break;
                }
            }
        }
    }
}

void panels_layout::save_layout()
{

    QSplitter *main_splitter = this->findChild<QSplitter *>("main_splitter");

    json_layout = {};

    add_json(main_splitter, 0, "None");

    jwrite("temp/layout.json", json_layout);
}

void panels_layout::load_layout()
{
}

void panels_layout::restore_default()
{
}