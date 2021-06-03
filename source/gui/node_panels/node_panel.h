#ifndef NODE_PANEL_H
#define NODE_PANEL_H

#include <QWidget>
#include <QJsonObject>
#include <QString>

#include <knob.h>
#include <project_struct.h>
#include <renderer.h>
#include <project_settings.h>

class node_panel : public QWidget
{
protected:
    QJsonObject knob_data;
    QJsonObject *params;

    QWidget *_vinacomp;
    renderer *_renderer;
    QWidget *_node_view;
    project_struct *project;
    project_settings *_project_settings;
    QString name;
    QWidget *_trim_panel;
    QGraphicsItem *this_node;

    knob *get_knob(QString name) const;
    void update_value(QJsonValue value);
    QJsonValue get_param_value() const;
    inline QJsonValue get_default() const;

public:
    node_panel();
    ~node_panel();

    void setup_env(QWidget *_trim_panel, QWidget *_vinacomp,
                   QJsonObject *params, QJsonObject knob_data, QString name,
                   QWidget *_node_view, QGraphicsItem *this_node);

    virtual void setup_knobs(QMap<QString, QVBoxLayout *> layouts);
    virtual void changed(knob *_knob);
    virtual void close();
    virtual QString get_script() const;
    inline QString get_name() const;
};

inline QJsonValue node_panel::get_default() const
{
    return knob_data.value("default");
}

inline QString node_panel::get_name() const
{
    return name;
}

#endif // NODE_PANEL_H
