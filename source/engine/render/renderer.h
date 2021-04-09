#ifndef RENDERER_H
#define RENDERER_H

#include <QMutex>
#include <QString>
#include <QThread>
#include <QWidget>

#include <project_struct.h>
#include <util.h>

#include <frame_range_node.h>
#include <node_engine.h>
#include <read_node.h>
#include <time_offset_node.h>

class renderer_thread : public QObject
{
    Q_OBJECT
private:
    project_struct *project;

    QMap<QString, node_engine *> nodes;
    read_node *read;
    time_offset_node *time_offset;
    frame_range_node *_frame_range;

    void recursive_render(render_data *rdata);

public:
    renderer_thread(project_struct *_project);
    ~renderer_thread();

    pair<int, int> get_frame_range(QString node_name) const;
    QString get_input_node(QString node_name, int input = 0) const;

    void run_render(render_data *rdata, int render_id);

signals:
    void finished_render(int render_id);
};

class renderer : public QObject
{
    Q_OBJECT
    QThread thread;

private:
    renderer_thread *_renderer_thread;
    bool rendering;
    int render_id;

    render_data last_rdata;
    render_data *rdata_thread;

public:
    renderer(project_struct *project);

    void render(render_data rdata);
    void thread_finished_render(int render_id);

signals:
    void thread_run_render(render_data *rdata, int render_id);
    void finished_render(render_data rdata);
};

#endif // RENDERER_H
