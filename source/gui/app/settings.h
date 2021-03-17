#ifndef SETTINGS_HPP
#define SETTINGS_HPP

// QT5
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

class settings : public QWidget
{
private:
    QVBoxLayout *layout;

public:
    settings(/* args */);
    ~settings();
};

#endif // SETTINGS_HPP
