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
