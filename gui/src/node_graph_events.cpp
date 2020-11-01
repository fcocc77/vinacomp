#include <node_graph.hpp>

void node_graph::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (!qt::alt() && event->button() == Qt::LeftButton)
    {
        QGraphicsItem *item = scene->itemAt(mapToScene(event->pos()), QTransform());
        QString item_name = item->data(0).toString();

        // impide la seleccion de nodos si se hizo el click en un link
        if (item_name != "link")
        {
            node *_node = get_node_from_position(event->pos());
            if (_node)
            {
                if (!qt::shift())
                    if (!_node->is_selected())
                        select_all(false);

                select_node(_node->get_name(), true);
            }
            else
            {
                if (!qt::shift())
                    select_all(false);
            }
        }

        // si el click no fue en un nodo, comienza el area de seleccion
        if (!item)
            selecting = true;
    }

    // Seleccionamos el QGraphicsItem de todos los nodos seleccionados, ya que esta selección
    // es la que nos permite mover todos los nodos juntos.
    for (node *selected_node : *selected_nodes)
        selected_node->setSelected(true);
    //
    //

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

    if (!qt::alt())
        this->refresh_selected_nodes();

    graphics_view::mouseMoveEvent(event);
}
