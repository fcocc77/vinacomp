#ifndef KNOB_FILE_HPP
#define KNOB_FILE_HPP

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>

class knob_file : public knob
{
    Q_OBJECT
private:
    QPushButton *file_open;
    QLineEdit *filename;

    void open_file();
    void restore_param() override;

public:
    knob_file(knob_props props, QString file_path = "");
    ~knob_file();
signals:
    void changed(QString file_path);
};

#endif // KNOB_FILE_HPP
