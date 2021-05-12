#ifndef SCRIPT_EDITOR_HPP
#define SCRIPT_EDITOR_HPP

// los .h de Qt definen "slots" para otra cosa,
// si esta definido 'slots' , 'Python.h' da conflicto
#ifdef slots
#undef slots
#endif
#include "Python.h"
//

#include <stdio.h>

// QT5
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

// Libs
#include <QCodeEditor.h>
#include <QPythonHighlighter.h>

class script_editor : public QWidget
{
private:
    void setup_ui();
    QWidget *tools_setup_ui();
    QCodeEditor *code_editor();

    QCodeEditor *editor;
    QJsonObject *project;
    QTextEdit *output;
    QWidget *node_graph;

    bool expression_editor;
    QWidget *expression_editor_bar;

    void run_script();
    void append_output(QString text, QColor color = Qt::white);
    QWidget *set_expression_editor_bar();

    // python
    void python_initialize();
    QString python_run(QString command);
    std::string std_out_err;
    PyObject *python_module;
    //

public:
    script_editor(QJsonObject *_project, QWidget *_node_graph,
                  bool expression_editor = false);
    ~script_editor();

    void open_script_from_project();
};

#endif // SCRIPT_EDITOR_HPP
