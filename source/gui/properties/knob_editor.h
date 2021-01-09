#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include <tools.h>
#include <action.h>

class knob_editor : public QWidget
{
private:
	QVBoxLayout *layout;
	QTreeWidget *knobs_list;
public:
	knob_editor();
	~knob_editor();
};



#endif // KNOB_EDITOR_H

