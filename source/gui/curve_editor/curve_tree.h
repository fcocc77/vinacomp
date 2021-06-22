#ifndef CURVE_TREE_H
#define CURVE_TREE_H

#include <QMouseEvent>
#include <QTreeWidget>

class curve_tree : public QTreeWidget
{
    Q_OBJECT
private:
    QTreeWidgetItem *get_node_item(QString item_name) const;
    QTreeWidgetItem *get_param_item(QTreeWidgetItem *node_item,
                                    QString param_name) const;

    QList<QTreeWidgetItem *> get_children(QTreeWidgetItem *item) const;
    void mousePressEvent(QMouseEvent *event) override;

public:
    curve_tree();

    void add_item(QString node_name, QString param_name, QString dimension_name,
                  QColor color);

    void delete_item(QString node_name, QString param_name = "",
                     QString dimension_name = "");

signals:
    void clicked(QString node_name, QList<QStringList> params);
    void outside_clicked();
};

#endif // CURVE_TREE_H
