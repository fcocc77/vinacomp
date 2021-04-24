#include <node_view.h>

void node_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();
    clicked();

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

        // si el click no fue en un nodo o es un backdrop, comienza el area de seleccion
        if (item)
        {
            node *clicked_node = dynamic_cast<node *>(item);
            if (clicked_node)
                if (clicked_node->get_type() == "backdrop")
                    selecting = true;
        }
        else
            selecting = true;
        //
    }

    node_rename_edit->hide();

    graphics_view::mousePressEvent(event);
}

void node_view::mouseReleaseEvent(QMouseEvent *event)
{
    selecting = false;
    selection_box->setVisible(false);

    connect_node(event->pos());
    connect_output_link(event->pos());

    graphics_view::mouseReleaseEvent(event);
}

void node_view::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting)
        if (!qt::alt())
            select_nodes_by_area(mapToScene(event->pos()));

    graphics_view::mouseMoveEvent(event);
}

void node_view::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        set_visible_ghost_dots(true);
    if (event->key() == Qt::Key_F)
        fit_view_to_nodes();
    if (event->key() == Qt::Key_1)
        connect_to_viewer();

    graphics_view::keyPressEvent(event);
}

void node_view::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        set_visible_ghost_dots(false);

    graphics_view::keyReleaseEvent(event);
}

void node_view::focusOutEvent(QFocusEvent *event)
{
    set_visible_ghost_dots(false);
}

void node_view::contextMenuEvent(QContextMenuEvent *event)
{
    right_click();
}
