#ifndef NODE_VIEWER_H
#define NODE_VIEWER_H

#include <node_rect.h>
#include <viewer.h>

class node_viewer : public node_rect
{
private:
    viewer *_viewer;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
    node_viewer(node_props _props, QMap<QString, node *> *_selected_nodes,
                QWidget *_node_graph);
    ~node_viewer();

    inline viewer *get_viewer() const;
    void make_viewer();
};

inline viewer *node_viewer::get_viewer() const
{
    return _viewer;
}

#endif // NODE_VIEWER_H
