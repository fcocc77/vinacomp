#include <node_graph.h>
#include <nodes_bar.h>

nodes_bar::nodes_bar(QWidget *__node_graph, maker *__maker, nodes_load *_nodes)
    : _maker(__maker)
    , nodes(_nodes)
    , _node_graph(__node_graph)

{
    this->setObjectName("nodes_bar");
    layout = new QHBoxLayout(this);
    layout->setMargin(4);

    QWidget *nodes_widget = new QWidget;
    nodes_widget->setObjectName("nodes_widget");
    nodes_layout = new QHBoxLayout(nodes_widget);
    nodes_layout->setMargin(0);

    add_menu("image", "image");
    add_menu("draw", "brush");
    add_menu("time", "time");
    add_menu("channel", "channel");
    add_menu("color", "color");
    add_menu("filter", "filter");
    add_menu("keyer", "keyer");
    add_menu("merge", "merge");
    add_menu("transform", "transform");
    add_menu("other", "other");

    for (auto binary : nodes->get_ofx()->get_binaries())
        add_menu(binary.name, binary.icon);

    update_py_plugins();

    find_node_edit = new QLineEdit();
    QLabel *find_node_label = new QLabel("Search Node");

    connect(find_node_edit, &QLineEdit::textChanged, this,
            &nodes_bar::search_changed);

    node_graph *___node_graph = static_cast<node_graph *>(_node_graph);

    // Layout
    add_widget(nodes_widget);

    add_stretch();

    add_widget(find_node_label);
    add_widget(find_node_edit);

    add_stretch();

    add_action(___node_graph->show_expressions_links);
    add_action(___node_graph->show_grid_action);
}

nodes_bar::~nodes_bar() {}

menu *nodes_bar::add_menu(QString group, QString icon_group)
{
    button *popup_button = new button(this);
    popup_button->setToolTip(group);
    popup_button->set_icon(icon_group);
    popup_button->setObjectName("nodes_bar_button");

    menu *_menu = new menu(this, popup_button);

    connect(popup_button, &QPushButton::pressed, this, [=] {
        popup_button->set_hover_icon();
        _menu->exec(popup_button->mapToGlobal({0, this->height() - 4}));
    });

    for (QJsonValue value : nodes->get_effects(group))
    {
        QJsonObject effect = value.toObject();
        QString label = effect["label"].toString();
        QString id = effect["id"].toString();
        QString icon = effect["icon"].toString();

        QString shortcut;
        if (id == "grade")
            shortcut = "G";
        else if (id == "transform")
            shortcut = "T";
        else if (id == "blur")
            shortcut = "B";
        else if (id == "merge")
            shortcut = "M";
        else if (id == "read")
            shortcut = "R";
        else if (id == "write")
            shortcut = "W";
        else if (id == "copy")
            shortcut = "K";
        else if (id == "roto")
            shortcut = "O";

        action *effect_action = new action(label, shortcut, icon);
        _menu->add_action(effect_action);

        effect_action->connect_to(_node_graph,
                                  [=]() { _maker->create_fx(id); });
    }

    nodes_layout->addWidget(popup_button);

    return _menu;
}

void nodes_bar::update_py_plugins()
{
    for (menu *_menu : py_plugins_menus)
        delete _menu;

    py_plugins_menus.clear();

    for (auto py_plugins_group : nodes->get_py_plugins_groups())
    {
        menu *_menu = add_menu(py_plugins_group.name, py_plugins_group.icon);
        py_plugins_menus.push_back(_menu);
    }
}

menu::menu(QWidget *parent, button *__button)
    : QMenu(parent)
    , popup_button(__button){};

menu::~menu()
{
    delete popup_button;

    for (action *_action : actions)
        delete _action;
}

void menu::add_action(action *_action)
{
    addAction(_action);
    actions.push_back(_action);
}

void menu::hideEvent(QHideEvent *event)
{
    popup_button->set_normal_icon();
    QMenu::hideEvent(event);
}

void menu::mouseMoveEvent(QMouseEvent *event)
{
    // si el boton de este menu no es igual al que esta bajo el cursor,
    // cierra este menu y abre el que corresponde.
    QPoint global_position = this->mapToGlobal(event->pos());

    QWidget *widget = qApp->widgetAt(global_position);
    button *_button = static_cast<button *>(widget);
    if (_button)
        if (_button->objectName() == "nodes_bar_button")
            if (_button != popup_button)
            {
                this->hide();

                _button->set_hover_icon();
                _button->click();
            }

    QMenu::mouseMoveEvent(event);
}
