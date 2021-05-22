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
    project_struct *project;
    QWidget *panel;
    QWidget *vinacomp;
    QList<QWidget *> *viewers_gl;
    QGraphicsItem *this_node;
    QWidget *knob_editor;
    QJsonObject *params;
    node_gui *_node_gui;
    QMap<QString, knob *> *knobs;
    QJsonArray knobs_array;
    QVBoxLayout *layout;
};

void setup_knobs(setup_knobs_props props);

#endif // SETUP_KNOBS_H
