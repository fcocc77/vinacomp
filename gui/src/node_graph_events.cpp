#include <node_graph.hpp>

void node_graph::mouseReleaseEvent(QMouseEvent *event)
{
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
        select_all(false);

        QString node_name = get_node_name_from_position(event->pos());
        select_node(node_name, true);
    }

    node_rename_edit->hide();

    graphics_view::mousePressEvent(event);
}