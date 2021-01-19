#include <node_graph.h>

node_graph::node_graph(
	QWidget *_vinacomp,
    project_struct *_project,
    properties *_properties)

	: project(_project)
{

    this->setObjectName("node_graph");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    nodes_loaded = new nodes_load();

    _node_view = new node_view(_vinacomp, _properties, nodes_loaded);
    _maker = new maker(_vinacomp, _properties, nodes_loaded, _node_view);
    _nodes_bar = new nodes_bar(_maker, nodes_loaded);

    layout->addWidget(_nodes_bar);
    layout->addWidget(_node_view);
}

node_graph::~node_graph()
{
}

node_view *node_graph::get_node_view() const
{
    return _node_view;
}

maker *node_graph::get_maker() const
{
    return _maker;
}

void node_graph::update_node_position_from_project()
{
	// ya que la posicion no es importante para en renderizado,
	// la posicion de lo nodos del proyecto no se actualiza, dinamicamente
	// asi que solo se actualiza cuando se guarda el proyecto.
	for (node *_node : *_node_view->get_nodes())
		project->nodes[_node->get_name()].pos = {_node->x(), _node->y()};
}

void node_graph::restore_tree()
{
	for (QString name : project->nodes.keys())
	{
		auto node = project->nodes.value(name);

		_node_view->create_node(
			name,
			node.color,
			node.type,
			node.pos
		);
	}

	// conecta todos los nodos
	for (QString name : project->nodes.keys())
	{
		auto node_data = project->nodes.value(name);
		node *_node = _node_view->get_nodes()->value(name);

		for (QString index_name : node_data.inputs.keys())
		{
			QString connected_node = node_data.inputs.value(index_name).toString();
			node *node_to_connect = _node_view->get_node(connected_node);

			int index = index_name.replace("in", "").toInt();

			node_link *link = _node_view->get_node_link(_node, index);
			if (link)
				link->connect_node(node_to_connect);
		}
	}
}

QJsonObject node_graph::get_scene_data() const
{
    QJsonObject matrix = {
        {"m11", _node_view->transform().m11()},
        {"m12", _node_view->transform().m12()},
        {"m13", _node_view->transform().m13()},
        {"m21", _node_view->transform().m21()},
        {"m22", _node_view->transform().m22()},
        {"m23", _node_view->transform().m23()},
        {"m31", _node_view->transform().m31()},
        {"m32", _node_view->transform().m32()},
        {"m33", _node_view->transform().m33()}};

    QJsonObject scene_data = {};

    scene_data["matrix"] = matrix;

    return scene_data;
}

void node_graph::restore_scene_data(QJsonObject scene_data)
{
    QJsonObject matrix = scene_data["matrix"].toObject();

    QTransform _transform(
        matrix["m11"].toDouble(),
        matrix["m12"].toDouble(),
        matrix["m13"].toDouble(),
        matrix["m21"].toDouble(),
        matrix["m22"].toDouble(),
        matrix["m23"].toDouble(),
        matrix["m31"].toDouble(),
        matrix["m32"].toDouble(),
        matrix["m33"].toDouble());

    _node_view->setTransform(_transform);
}
