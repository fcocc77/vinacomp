#include <node_viewer.h>
#include <vinacomp.h>

node_viewer::node_viewer(node_props _props,
                         QMap<QString, node *> *_selected_nodes,
                         QWidget *_node_graph)

    : node_rect(_props, _selected_nodes, _node_graph)
    , _viewer(nullptr)
{
}

node_viewer::~node_viewer()
{
    vinacomp *__vinacomp = static_cast<vinacomp *>(props.vinacomp);

    if (_viewer)
    {
        __vinacomp->get_viewers()->remove(_viewer->get_name());
        __vinacomp->get_panels_layout()->delete_viewer(_viewer);
        delete _viewer;
    }
    // falta borrar del viewer de la lista de viewer de vinacomp
    // borrar viewer del panels_layout y de la lista del panel
}

void node_viewer::make_viewer()
{
    vinacomp *__vinacomp = static_cast<vinacomp *>(props.vinacomp);
    if (type == "viewer")
    {
        if (!_viewer)
        {
            _viewer = new viewer(name, props.project,
                                 __vinacomp->get_renderer(), props.vinacomp);

            __vinacomp->get_viewers()->insert(name, _viewer);
        }
        __vinacomp->get_panels_layout()->add_viewer(_viewer);
    }
}

void node_viewer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    make_viewer();
}
