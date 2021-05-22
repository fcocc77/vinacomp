#ifndef SETUP_KNOBS_H
#define SETUP_KNOBS_H

#include <QGraphicsItem>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QWidget>

#include <node_gui.h>
#include <project_struct.h>

struct setup_knobs_props
{
    project_struct *project = nullptr;
    QWidget *panel = nullptr;
    QWidget *vinacomp = nullptr;
    QList<QWidget *> *viewers_gl = nullptr;
    QGraphicsItem *this_node = nullptr;
    QWidget *knob_editor = nullptr;
    QJsonObject *params = nullptr;
    node_gui *_node_gui = nullptr;
    QMap<QString, knob *> *knobs;
    QJsonArray knobs_array;
    QVBoxLayout *layout;
};

void setup_knobs(setup_knobs_props props);

#endif // SETUP_KNOBS_H
