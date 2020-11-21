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
private:
    void open_file();

public:
    knob_file(/* args */);
    ~knob_file();
};

#endif //KNOB_FILE_HPP