#ifndef SCRIPT_EDITOR_HPP
#define SCRIPT_EDITOR_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QCodeEditor.hpp>
#include <QPythonHighlighter.hpp>

#include <qt.hpp>

class script_editor : public QWidget
{
private:
    void setup_ui();
    QWidget *tools_setup_ui();
    QCodeEditor *code_editor();

public:
    script_editor(/* args */);
    ~script_editor();
};

#endif // SCRIPT_EDITOR_HPP