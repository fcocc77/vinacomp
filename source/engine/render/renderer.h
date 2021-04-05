#ifndef RENDERER_H
#define RENDERER_H

#include <QString>
#include <QWidget>

#include <project_struct.h>
#include <util.h>

#include <node_engine.h>
#include <read_node.h>
#include <time_offset_node.h>
#include <frame_range_node.h>

class renderer : public QWidget
{
private:
    project_struct *project;

    QMap<QString, node_engine *> nodes;
    read_node *read;
    time_offset_node *time_offset;
    frame_range_node *_frame_range;

public:
    renderer(project_struct *_project);
    ~renderer();

    void render(render_data *rdata);

    pair<int, int> get_frame_range(QString node_name) const;
    QString get_input_node(QString node_name, int input = 0) const;
};

#endif // RENDERER_H
