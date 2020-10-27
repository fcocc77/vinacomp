#ifndef SCRIPT_EDITOR_HPP
#define SCRIPT_EDITOR_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QJsonObject>
#include <QSlider>
#include <QLabel>

// Libs
#include <QCodeEditor.hpp>
#include <QPythonHighlighter.hpp>

//VinaComp
#include <qt.hpp>

class script_editor : public QWidget
{
private:
    void setup_ui();
    QWidget *tools_setup_ui();
    QCodeEditor *code_editor();

    QCodeEditor *editor;
    QJsonObject *project;

public:
    script_editor(QJsonObject *_project);
    ~script_editor();

    void open_script_from_project();
};

#endif // SCRIPT_EDITOR_HPP