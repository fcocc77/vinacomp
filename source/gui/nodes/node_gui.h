#ifndef NODE_GUI_H
#define NODE_GUI_H

#include <QWidget>
#include <QJsonObject>
#include <QString>

#include <knob.h>
#include <project_struct.h>
#include <renderer.h>
#include <project_settings.h>

class node_gui : public QWidget
{
private:
    QJsonObject knob_data;
    QJsonObject *params;

protected:
    QWidget *_vinacomp;
    renderer *_renderer;
    project_struct *project;
    project_settings *_project_settings;
    QString name;
    QWidget *_trim_panel;

    knob *get_knob(QString name) const;
    void update_value(QJsonValue value);
    QJsonValue get_param_value() const;
    inline QJsonValue get_default() const;

public:
    node_gui();
    ~node_gui();

    void setup_env(QWidget *_trim_panel, QWidget *_vinacomp, QJsonObject *params,
               QJsonObject knob_data, QString name);

    virtual void setup_knobs(QMap<QString, QVBoxLayout *> layouts);
    virtual void changed(knob *_knob);
    virtual void close();
    inline QString get_name() const;
};

inline QJsonValue node_gui::get_default() const
{
    return knob_data.value("default");
}

inline QString node_gui::get_name() const
{
    return name;
}

#endif // NODE_GUI_H
