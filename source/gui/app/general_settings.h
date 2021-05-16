#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

// QT5
#include <QTreeWidget>
#include <QVBoxLayout>

#include <settings.h>

class general_settings : public settings
{
private:
    void setup_general();
    void setup_auto_save();
    void setup_plugins();
    void setup_appearance();

    void save_settings() override;

public:
    general_settings(QWidget *_vinacomp);
    ~general_settings();
};

#endif // GENERAL_SETTINGS_H
