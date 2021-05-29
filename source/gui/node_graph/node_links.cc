#include "node.h"
#include <node_links.h>
#include <util.h>

node_links::node_links(QGraphicsScene *_scene, QWidget *_node_view,
                       QGraphicsItem *_this_node)

    : this_node(_this_node)
    , scene(_scene)
{
}

node_links::~node_links()
{
    for (node_link *link : links)
        delete link;

    links.clear();
}

void node_links::make_links(int count)
{
    // crea una cantidad de links, y si ya existen algunos links completa los
    // que faltan.

    if (count <= links.count())
        return;

    int extra_links = count - links.count();

    for (int i = 0; i < extra_links; i++)
        links.push_back(new node_link(scene));
}

void node_links::refresh()
{
    node *_this_node = static_cast<node *>(this_node);
    auto handler_nodes = _this_node->get_handler_nodes();

    if (handler_nodes.isEmpty())
        return;

    make_links(handler_nodes.count());

    for (int i = 0; i < handler_nodes.count(); i++)
    {
        node *handler_node = handler_nodes.value(i);

        QPointF src_pos = _this_node->get_center_position();
        QPointF dst_pos = handler_node->get_center_position();

        links.value(i)->set_line(src_pos, dst_pos);
    }
}

void node_links::set_visible(bool _visible)
{
    for (node_link *link : links)
        link->set_visible(_visible);

    refresh();
}

void node_links::enable_links(int links_count)
{
    disable_all_links();

    if (links_count > links.count())
        links_count = links.count();

    for (int i = 0; i < links_count; i++)
        links.value(i)->set_disable(false);

    if (links_count)
        set_visible(true);

    refresh();
}

void node_links::disable_all_links()
{
    for (node_link *link : links)
        link->set_disable(true);
}
