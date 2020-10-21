#ifndef VINACOMP_H
#define VINACOMP_H

// QT5
#include <QMainWindow>
#include <QSplitter>
#include <QMenuBar>

// VINA COMP
#include <viewer.hpp>
#include <node_graph.hpp>
#include <util.hpp>
#include <panels_layout.hpp>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    viewer *_viewer;
    panels_layout *_layout;

public:
    vinacomp(QWidget *parent = 0);
    ~vinacomp();

    void setup_ui();
    void setup_style();
    void main_menu();
};

#endif // VINACOMP_H