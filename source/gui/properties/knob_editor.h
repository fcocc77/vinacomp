#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

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

