#include <panels_layout.hpp>

panels_layout::panels_layout(/* args */)
{
    this->setObjectName("panels_layout");

    // QList <QSplitter*> splitters()

    panel *_panel = new panel();

    QWidget *central_widget = new QWidget();
    qt::add_widget(this, _panel);

    restore_default_action = new QAction("Restore Default Layout");
    save_current_action = new QAction("Save Current Layout");

    connect(save_current_action, &QAction::triggered, this, &panels_layout::save_layout);
}

panels_layout::~panels_layout()
{
}

void panels_layout::save_layout()
{
    QJsonObject json_layout = {
        {"hola", 20}};

    for (int i = 0; i < 10; i++)
    {
        QString name_a = "splitter:" + QString::number(i) + ":a";
        QString name_b = "splitter:" + QString::number(i) + ":b";

        QWidget *splitter_a = this->findChild<QWidget *>(name_a);

        print(splitter_a);
    }
    print("-----------");

    json_layout["container_a"] = "container_a";
    json_layout["container_b"] = "container_b";

    jwrite("temp/layout.json", json_layout);
}

void panels_layout::restore()
{
}

void panels_layout::restore_default()
{
}