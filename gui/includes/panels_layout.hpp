#ifndef PANELS_LAYOUT_HPP
#define PANELS_LAYOUT_HPP

#include <QWidget>
#include <panel.hpp>
#include <node_graph.hpp>
#include <viewer.hpp>

class panels_layout : public QWidget
{
private:
    void save_layout();
    void load_layout();
    void restore_default();

    panel *get_child_panel(QSplitter *__splitter, QString _letter);

    void save_json_layout(QSplitter *splitter, int deep = 0, QString letter = "", QStringList parents = {});
    void load_splitter(QJsonObject splitter_obj, panel *panel_a);
    QJsonObject json_layout;

    panel *first_panel;

public:
    panels_layout(node_graph *_node_graph, viewer *_viewer);
    ~panels_layout();

    QList<QSplitter *> *splitters;

    QAction *restore_default_action;
    QAction *save_current_action;
};

#endif // PANELS_LAYOUT_HPP