#include <node_graph.h>

void node_graph::init_menu()
{
    menu = new QMenu(this);

    action *open_group_action = new action("Open Group", "", "group");
    menu->addAction(open_group_action);

    connect(_node_view, &node_view::right_click, this, [=]() {
        menu->move(QCursor::pos());
        menu->show();
    });
}
