#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QMap>
#include <QPointF>
#include <QRectF>

struct node_struct
{
    QString name;
    QColor color;
    QString type;
    QString tips;
    QPointF pos;
    QJsonObject *params;
    QJsonObject inputs;
    QJsonArray *custom_knobs;
    QJsonArray handler_nodes;
    QString script;
    bool plugin;

    // estos 2 atributos son solo para el backdrop
    QSize size;
    int z_value;
};

struct layer_struct {
    QString name;
    bool red;
    bool green;
    bool blue;
    bool alpha;
};

struct global_struct
{
    QList<layer_struct> layers;
    QRectF node_view_rect;
};

class project_struct
{
private:
    void create_base_children_for_group(node_struct node);
    void create_children_plugin(node_struct node, bool from_project);

    node_struct get_node_from_object(QString name, QJsonObject node_obj) const;

public:
    project_struct();
    ~project_struct();

    QMap<QString, node_struct> nodes;
    global_struct global;
    int frame;

    void insert_node(node_struct node, QJsonObject _params = {},
                     QJsonArray custom_knobs = {}, bool from_project = false);

    void delete_node(QString name);

    static QString replace_parent_name(QString node_name, QString parent_name,
                                       QString new_group_name);

    static QJsonObject replace_parent_name_to_params(QJsonObject params,
                                                     QString parent_name,
                                                     QString new_parent_name);

    static QJsonObject replace_parent_name_to_inputs(QJsonObject inputs,
                                                     QString parent_name,
                                                     QString new_parent_name);

    static QJsonArray replace_parent_name_to_handlers(QJsonArray handler_nodes,
                                                     QString parent_name,
                                                     QString new_parent_name);

    void replace_parent_name_to_node(node_struct *node, QString parent_name,
                                     QString new_parent_name);

    void replace_parent_name_to_children(QString parent_name,
                                         QString new_parent_name);

    void rename_node(QString name, QString new_name,
                     bool rename_children = true);

    void unlink_all_knobs(QString node_name);
    void unlink_param_from_handler(QString node_name, QString param_name);
    void unlink_param_from_slave(QString handler_node_name,
                                 QString handler_param_name,
                                 QString slave_node_name,
                                 QString slave_param_name);

    void rename_slave_link(node_struct *handler_node, QString salve_name,
                           QString new_slave_name);

    void rename_handler_link(node_struct *slave_node, QString handler_name,
                             QString new_handler_name);

    void rename_node_links(QString node_name, QString new_name);

    bool exist_handler_node_in_params(QString slave_node_name,
                                      QString handler_node_name) const;

    QStringList get_slaves_nodes(node_struct *node) const;
    QStringList get_handler_nodes(node_struct *node) const;

    void insert_input(QString src, QString dst, int index);
    void delete_input(QString node, int index);

    QJsonObject get_project_json() const;

    void save(QString project_path) const;
    void load(QString project_path);
    void load_from_json(QJsonObject project);

    static QJsonValue get_value_frame(QJsonObject *params, QString param_name,
                                      int frame);

    QList<node_struct> get_nodes_from_group(QString group_name) const;

    QList<node_struct> get_children_nodes(QString parent_name,
                                          bool recursive = false) const;

    void clear_nodes();
};

#endif // PROJECT_STRUCT_H
