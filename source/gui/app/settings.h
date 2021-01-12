#ifndef SETTINGS_HPP
#define SETTINGS_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>

class settings : public QWidget
{
private:
	QVBoxLayout *layout;
public:
    settings(/* args */);
    ~settings();
};

#endif // SETTINGS_HPP
