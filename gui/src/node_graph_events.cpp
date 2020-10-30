#include <node_graph.hpp>

void node_graph::mouseReleaseEvent(QMouseEvent *event)
{

    // si un enlace input de un nodo esta siendo arrastrado para  conectarlo a otro nodo,
    // 'link_connecting' no estara vacio y se determinara
    // si se conecta o no al nodo de destino.
    if (!link_connecting->empty())
    {
        QString node_name = link_connecting->value("name").toString();
        int link_index = link_connecting->value("index").toInt();

        node *from_node = nodes->value(node_name);
        node_link *link = get_node_link(from_node, link_index);

        node *to_node = get_node_from_position(event->pos());
        if (!to_node)
            link->disconnect_node();
        else
        {
            link->connect_node(to_node);
            to_node->add_output_node(from_node);
        }
        *link_connecting = {};
    }
    //
    //

    this->refresh_selected_nodes();

    graphics_view::mouseReleaseEvent(event);
}

void node_graph::mouseMoveEvent(QMouseEvent *event)
{
    this->refresh_selected_nodes();
    graphics_view::mouseMoveEvent(event);
}

void node_graph::mousePressEvent(QMouseEvent *event)
{
    if (!qt::alt())
    {
        if (!qt::shift())
            select_all(false);

        node *_node = get_node_from_position(event->pos());
        if (_node)
            select_node(_node->get_name(), true);
    }

    node_rename_edit->hide();

    graphics_view::mousePressEvent(event);
}