#ifndef KNOB_FILE_HPP
#define KNOB_FILE_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

#include <qt.h>
#include <knob.h>

class knob_file : public knob
{
	Q_OBJECT
private:
	QPushButton *file_open;
	QLineEdit *filename;

    void open_file();

public:
    knob_file(QString file_path = "");
    ~knob_file();
signals:
	void changed(QString file_path);
};

#endif //KNOB_FILE_HPP
