#include <node_graph.hpp>

void node_graph::mousePressEvent(QMouseEvent *event)
{
    if (!qt::alt() && event->button() == Qt::LeftButton)
    {
        QGraphicsItem *item = scene->itemAt(mapToScene(event->pos()), QTransform());
        QString item_name = item->data(0).toString();

        // impide la seleccion de nodos si se hizo el click en un link
        if (item_name != "link")
        {
            if (!qt::shift())
                select_all(false);

            node *_node = get_node_from_position(event->pos());
            if (_node)
                select_node(_node->get_name(), true);
        }

        // si el click no fue en un nodo, comienza el area de seleccion
        if (!item)
        {
            selecting = true;
            selection_start_point = mapToScene(event->pos());
        }
    }

    node_rename_edit->hide();

    graphics_view::mousePressEvent(event);
}

void node_graph::mouseReleaseEvent(QMouseEvent *event)
{
    selecting = false;

    connect_node(event->pos());

    this->refresh_selected_nodes();

    graphics_view::mouseReleaseEvent(event);
}

void node_graph::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting)
        if (!qt::alt())
            select_nodes_by_area(mapToScene(event->pos()));

    this->refresh_selected_nodes();
    graphics_view::mouseMoveEvent(event);
}
