#include <node_graph.h>

void node_graph::init_menu()
{
    general_menu = new QMenu(this);

    action *search_action = new action("Search", "Ctrl+F", "search");
    action *center_on_all_nodes =
        new action("Center on All Nodes", "F", "center");
    show_expressions_links =
        new action("Show Expressions Links", "Shift+E", "link_off");
    show_grid_action = new action("Show Grid", "", "grid");

    // paste nodes
    action *paste_nodes_action = new action("Paste Nodes", "Ctrl+V", "paste");
    paste_nodes_action->connect_to(this,
                                   [this]() { _node_view->paste_nodes(); });
    //

    show_expressions_links->set_checkable(true, "link");
    show_expressions_links->set_checked(true);

    show_expressions_links->connect_to(this, [=]() {
        bool visible = _node_view->are_visible_links();
        _node_view->set_visible_links(!visible);
    });

    general_menu->addAction(paste_nodes_action);
    general_menu->addSeparator();
    general_menu->addAction(search_action);
    general_menu->addSeparator();
    general_menu->addAction(show_expressions_links);
    general_menu->addAction(show_grid_action);
    general_menu->addAction(center_on_all_nodes);
    //

    // Menu para los nodos
    node_menu = new QMenu(this);
    connect(_node_view, &node_view::right_click, this, [=]() {
        if (_node_view->get_selected_nodes()->empty())
        {
            general_menu->move(QCursor::pos());
            general_menu->show();
        }
        else
        {
            node_menu->move(QCursor::pos());
            node_menu->show();
        }
    });

    // Node Actions
    action *copy_nodes_action = new action("Copy Nodes", "Ctrl+C", "copy");
    action *cut_nodes_action = new action("Cut Nodes", "Ctrl+X", "cut");
    action *remove_nodes_action =
        new action("Remove Nodes", "Backspace", "close");
    action *rename_node_action = new action("Rename Node", "N", "edit");
    action *extract_node_action =
        new action("Extract Node", "Ctrl+Shift+x", "");
    action *switch_inputs_a_b =
        new action("Switch Inputs A and B", "Shift+X", "switch_inputs_a_b");
    action *unlink_nodes_action = new action("Unlink Nodes", "", "link_off");
    action *duplicate_nodes_action =
        new action("Duplicate Nodes", "Ctrl+D", "copy");
    action *clone_nodes_action = new action("Clone Nodes", "Ctrl+K", "clone");
    action *disable_nodes_action = new action("Disable Nodes", "D", "disable");
    action *group_from_selection =
        new action("Group from Selection", "Ctrl+G", "group");
    action *open_group_action = new action("Open Group", "", "open");

    // Conecciones
    copy_nodes_action->connect_to(this, [this]() { _node_view->copy_nodes(); });
    remove_nodes_action->connect_to(
        this, [this]() { _node_view->delete_selected_nodes(); });
    extract_node_action->connect_to(
        this, [this]() { _node_view->extract_selected_nodes(); });

    switch_inputs_a_b->connect_to(
        this, [this]() { _node_view->switch_inputs_a_and_b(); });

    duplicate_nodes_action->connect_to(this, [this]() {
        _node_view->copy_nodes();
        _node_view->paste_nodes();
    });
    disable_nodes_action->connect_to(
        this, [this]() { _node_view->disable_selected_nodes(); });

    // Layout
    node_menu->addAction(copy_nodes_action);
    node_menu->addAction(cut_nodes_action);
    node_menu->addAction(paste_nodes_action);

    node_menu->addSeparator();

    node_menu->addAction(remove_nodes_action);
    node_menu->addAction(rename_node_action);
    node_menu->addAction(extract_node_action);
    node_menu->addAction(switch_inputs_a_b);

    node_menu->addSeparator();

    node_menu->addAction(duplicate_nodes_action);
    node_menu->addAction(clone_nodes_action);
    node_menu->addAction(disable_nodes_action);
    node_menu->addAction(unlink_nodes_action);

    node_menu->addSeparator();

    node_menu->addAction(group_from_selection);
    node_menu->addAction(open_group_action);
}
