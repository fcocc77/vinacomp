#include <node_view.h>

node_view::node_view(
	QWidget *__vinacomp,
    QJsonObject *_project,
    properties *__properties,
	nodes_load *_nodes_loaded)

    : _vinacomp(__vinacomp)
	, project(_project)
	, _properties(__properties)
	, nodes_loaded(_nodes_loaded)
{

    scene = new QGraphicsScene();

    // cuadro de cambio de nombre de nodo
    node_rename_edit = new QLineEdit(this);
    connect(node_rename_edit, &QLineEdit::returnPressed, this, [this] {
        change_node_name();
    });
    node_rename_edit->hide();
    //
    //

    this->setObjectName("node_view");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    // activacion de area de seleccion
    this->setDragMode(QGraphicsView::RubberBandDrag);
    //
    //

    nodes = new QMap<QString, node *>;
    selected_nodes = new QMap<QString, node *>;
    link_connecting = new QJsonObject();

    current_z_value = new int();

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);
    this->setRenderHint(QPainter::Antialiasing);

    setup_shortcut();
}

node_view::~node_view()
{
}

void node_view::setup_shortcut()
{
    qt::shortcut("Ctrl+A", this, [this]() {
        this->select_all(true);
    });

    qt::shortcut("N", this, [this]() {
        this->change_node_name_dialog();
    });

    qt::shortcut("L", this, [this]() {
        this->align_selected_nodes();
    });
}

void node_view::align_selected_nodes()
{
    if (selected_nodes->empty())
        return;

    // obtiene el primer item seleccionado para sacar la posicion del nodo
    node *first_node = selected_nodes->first();
    //

    int separation = 20;
    int pos_x = first_node->x();
    int pos_y = first_node->y();
    for (node *selected_node : *selected_nodes)
    {
        selected_node->set_position(pos_x, pos_y);
        pos_x += separation + selected_node->get_size().width();
    }
}

void node_view::rename_node(node *_node, QString name, QString new_name)
{
    _node->set_name(new_name);

    nodes->remove(name);
    nodes->insert(new_name, _node);

    selected_nodes->remove(name);
    selected_nodes->insert(new_name, _node);

    _node->refresh();
}

void node_view::change_node_name_dialog()
{
    if (selected_nodes->empty())
        return;

    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    node_rename_edit->setText(selected_node->get_name());
    QPoint position = this->mapFromGlobal(QCursor::pos());

    node_rename_edit->move(position);
    node_rename_edit->setFocus();
    node_rename_edit->show();
}

void node_view::change_node_name()
{
    // cambia el nombre al ultimo nodo seleccionado
    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    QString old_name = selected_node->get_name();
    QString new_name = node_rename_edit->text();

    rename_node(selected_node, old_name, new_name);

    node_rename_edit->hide();
}

node *node_view::create_node(
    QString name,
    QColor color,
	QString type,
    QPointF position,
    QString tips)
{
    if (position.isNull())
    {
        // crea el nodo en la posicion del puntero del mouse
        QPoint origin = this->mapFromGlobal(QCursor::pos());
        position = this->mapToScene(origin);
    }

    node *_node = new node(
        scene,
        current_z_value,
        link_connecting,
        selected_nodes,
        1,
        color,
		type,
		name,
		tips,
        _properties,
		_vinacomp,
		nodes_loaded
	);
    auto size = _node->get_size();
    _node->set_position(position.x() - (size.width() / 2), position.y() - (size.height() / 2));

    nodes->insert(name, _node);

    return _node;
}

void node_view::connect_node(QPoint position_node)
{
    // si un enlace input de un nodo esta siendo arrastrado para conectarlo a otro nodo,
    // 'link_connecting' no estara vacio y se determinara
    // si se conecta o no al nodo de destino.
    if (!link_connecting->empty())
    {
        QString node_name = link_connecting->value("name").toString();
        int link_index = link_connecting->value("index").toInt();

        node *from_node = get_node(node_name);
        node_link *link = get_node_link(from_node, link_index);

        node *to_node = get_node_from_position(position_node);
        if (!to_node)
            link->disconnect_node();
        else
        {
            // evita que se conecte asi mismo
            if (from_node != to_node)
                link->connect_node(to_node);
            else
                link->disconnect_node();
        }
        *link_connecting = {};
    }
    //
    //
}

node *node_view::get_node(QString name)
{
    return nodes->value(name);
}

void node_view::select_node(QString name, bool select)
{
    node *_node = get_node(name);
    if (_node == NULL)
        return;

    _node->set_selected(select);

    if (select)
        selected_nodes->insert(name, _node);
    else
        selected_nodes->remove(name);
}

node *node_view::get_node_from_position(QPoint position)
{
    // ya que el nodo esta compuesto por muchos hijos, al dar click puede ser un hijo,
    // y si es un hijo obtiene el nodo padre para poder extraer el nombre del nodo.
    QGraphicsItem *item = scene->itemAt(mapToScene(position), QTransform());
    if (!item)
        return NULL;

    QGraphicsItem *parent_item = item->parentItem();
    if (parent_item)
        item = parent_item;
    //
    //
    QString node_name = item->data(0).toString();

    return get_node(node_name);
}

node_link *node_view::get_node_link(node *_node, int link_index)
{
    if (!_node)
        return NULL;

    node_link *link = _node->get_links()->value(link_index);

    return link;
}

void node_view::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        select_node(_node->get_name(), select);
}

void node_view::select_nodes_by_area(QPointF selection_end_point)
{
    QPointF selection_start_point = mapToScene(click_position);

    QPainterPath rectangle;

    int start_x = selection_start_point.x();
    int start_y = selection_start_point.y();

    int end_x = selection_end_point.x();
    int end_y = selection_end_point.y();

    int width = end_x - start_x;
    int height = end_y - start_y;

    QRectF rect(start_x, start_y, width, height);
    rectangle.addRect(rect);

    if (!qt::shift())
        select_all(false);

    QList<QGraphicsItem *> selected_items = scene->items(rectangle);
    for (QGraphicsItem *item : selected_items)
    {
        QString node_name = item->data(0).toString();
        select_node(node_name, true);
    }
}

QMap<QString, node *> *node_view::get_nodes()
{
    return nodes;
}
