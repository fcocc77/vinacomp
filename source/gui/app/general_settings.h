#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

// QT5
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QTreeWidget>

#include <settings.h>
#include <knob.h>

class general_settings : public settings
{
private:
    QJsonObject *data;

    void setup_general();
    void setup_auto_save();
    void setup_appearance();

    void save_settings() override;
    void restore_settings();

    // Plugins
    QList<QTreeWidgetItem *> plugin_items;
    QTreeWidget *plugin_tree;
    void setup_plugins();
    void load_plugins();
    void update_plugin_tree();
    void delete_plugin();

    void showEvent(QShowEvent *event) override;

public:
    general_settings(QWidget *_vinacomp);
    ~general_settings();
};

#endif // GENERAL_SETTINGS_H
