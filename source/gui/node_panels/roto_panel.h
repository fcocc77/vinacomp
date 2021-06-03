#ifndef ROTO_PANEL_H
#define ROTO_PANEL_H

#include <QTreeWidget>

#include <node_panel.h>
#include <button.h>

class roto_panel : public node_panel
{
private:
    QTreeWidget *tree;

    QWidget *buttons;

    button *add_button, *remove_button;
    void setup_knobs(QMap<QString, QVBoxLayout *> layouts) override;

public:
    roto_panel();
    ~roto_panel();

    void changed(knob *_knob) override;
};

#endif // ROTO_PANEL_H
