#ifndef SCRIPT_EDITOR_HPP
#define SCRIPT_EDITOR_HPP

// los .h de Qt definen "slots" para otra cosa,
// si esta definido 'slots' , 'Python.h' da conflicto
#ifdef slots
#undef slots
#endif
#include "Python.h"
//

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QJsonObject>
#include <QLabel>
#include <QScrollBar>

// Libs
#include <QCodeEditor.h>
#include <QPythonHighlighter.h>

//VinaComp
#include <qt.h>
#include <slider.h>
#include <util.h>
#include <action.h>

class script_editor : public QWidget
{
private:
    void setup_ui();
    QWidget *tools_setup_ui();
    QCodeEditor *code_editor();

    QCodeEditor *editor;
    QJsonObject *project;
    QTextEdit *output;

    void run_script();
    void append_output(QString text, QColor color = Qt::white);

    // python
    void python_initialize();
    QString python_run(QString command);
    string std_out_err;
    PyObject *python_module;
    //

public:
    script_editor(QJsonObject *_project);
    ~script_editor();

    void open_script_from_project();
};

#endif // SCRIPT_EDITOR_HPP