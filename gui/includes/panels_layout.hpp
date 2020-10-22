#ifndef PANELS_LAYOUT_HPP
#define PANELS_LAYOUT_HPP

#include <QWidget>
#include <panel.hpp>

class panels_layout : public QWidget
{
private:
    void save_layout();
    void load_layout();
    void restore_default();


    void add_json(QSplitter *splitter, int deep, QString letter);
    QJsonObject json_layout;

public:
    panels_layout(/* args */);
    ~panels_layout();

    QList<QSplitter *> *splitters;

    QAction *restore_default_action;
    QAction *save_current_action;
};

#endif // PANELS_LAYOUT_HPP