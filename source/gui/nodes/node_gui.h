#ifndef NODE_GUI_H
#define NODE_GUI_H

#include <QWidget>

#include <knob.h>
#include <project_struct.h>
#include <renderer.h>

#include <util.h>

class node_gui : public QWidget
{
private:
    QWidget *_trim_panel;

public:
    node_gui();
    ~node_gui();

    void setup(QWidget *_trim_panel, QWidget *_vinacomp, QString name);
    virtual void changed(QString param_name);

protected:
    renderer *_renderer;
    project_struct *project;
    QString name;

    knob *get_knob(QString name) const;
};

#endif // NODE_GUI_H
