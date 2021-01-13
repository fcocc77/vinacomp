#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

#include <viewer.h>
#include <knob_floating.h>
#include <knob_dimensions.h>

class project_settings : public QWidget
{
private:
	QVBoxLayout *layout;
	viewer *_viewer;

	int first_frame, last_frame;
	float proxy_scale;
	QTextEdit *comment_text_edit;
public:
	project_settings(
		viewer *_viewer
	);
	~project_settings();
};

#endif // PROJECT_SETTINGS_H
