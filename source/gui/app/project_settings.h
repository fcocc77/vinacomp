#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>

class project_settings : public QWidget
{
private:
	QVBoxLayout *layout;
public:
	project_settings();
	~project_settings();

};

#endif // PROJECT_SETTINGS_H
