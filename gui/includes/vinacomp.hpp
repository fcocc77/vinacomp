#ifndef VINACOMP_H
#define VINACOMP_H

// QT5
#include <QMainWindow>
#include <QSplitter>

// VINA COMP
#include <viewer.hpp>
#include <node_graph.hpp>
#include <util.hpp>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    viewer *_viewer;

public:
    vinacomp(QWidget *parent = 0);
    ~vinacomp();

    void setup_ui();
    void setup_style();
};

#endif // VINACOMP_H