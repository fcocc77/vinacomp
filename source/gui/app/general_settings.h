#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

// QT5
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QJsonObject>

#include <settings.h>

class general_settings : public settings
{
private:
    QJsonObject data;

    void *auto_save_enable_pointer;
    void *auto_save_delay_pointer;

    void setup_general();
    void setup_auto_save();
    void setup_plugins();
    void setup_appearance();

    void save_settings() override;
    void restore_settings();

    void showEvent(QShowEvent *event) override;

public:
    general_settings(QWidget *_vinacomp);
    ~general_settings();
};

#endif // GENERAL_SETTINGS_H
