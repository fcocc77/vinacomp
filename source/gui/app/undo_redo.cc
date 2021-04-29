#include <vinacomp.h>

void vinacomp::add_history()
{
    int max_record = 10;

    _node_graph->save_nodes_attributes_to_project();
    history.push_back(project->get_project_json());

    if (history.count() > max_record)
        history.removeFirst();

    current_history = history.count() - 1;
}

void vinacomp::restore_history(QJsonObject record)
{
    if (record.empty())
        return;

    _node_graph->clear_tree();

    project->load_from_json(record);
    _node_graph->restore_tree();
}

void vinacomp::undo()
{
    if (current_history < 0)
        return;

    restore_history(history.value(current_history));
    current_history--;
}

void vinacomp::redo()
{
    if (current_history >= history.count())
        return;

    current_history++;
    restore_history(history.value(current_history));
}
