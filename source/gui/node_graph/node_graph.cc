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

QJsonObject node_graph::get_tree() const
{
    // genera un arbol con todos los nodos con su informacion,
    // para guardarla en el proyecto.
    QJsonObject tree = {};

    // for (node *_node : *_node_view->get_nodes())
    // {
        // QJsonArray inputs = {};

        // for (node_link *link : *_node->get_links())
        // {
            // node *connected_node = dynamic_cast<node *>(link->get_connected_node());
            // QString _connected_node = "NULL";
            // if (connected_node)
                // _connected_node = connected_node->get_name();

            // QJsonObject link_data = {
                // {"connected_node", _connected_node}};

            // inputs.push_back(link_data);
        // }

        // QColor color = _node->get_color();

        // QJsonObject data = {
            // {"pos", QJsonArray{_node->x(), _node->y()}},
            // {"color", QJsonArray{color.red(), color.green(), color.blue()}},
            // {"inputs", inputs},
			// {"params", *_node->get_parameters_data()},
			// {"type", _node->get_type()}
		// };

        // tree[_node->get_name()] = data;
    // }

    return tree;
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
    // for (QString name : _nodes.keys())
    // {
        // QJsonObject data = _nodes.value(name).toObject();
		// QString type = data["type"].toString();

        // QJsonArray color = data["color"].toArray();
        // int red = color[0].toInt();
        // int green = color[1].toInt();
        // int blue = color[2].toInt();

        // QPointF position = {data["pos"].toArray()[0].toDouble(),
                            // data["pos"].toArray()[1].toDouble()};


        // _node_view->create_node(
            // name,
            // QColor(red, green, blue),
			// type,
            // position,
			// data.value("params").toObject()
		// );
    // }

    // // conecta todos los nodos
    // for (QString name : _nodes.keys())
    // {
        // QJsonObject data = _nodes.value(name).toObject();
        // node *_node = _node_view->get_nodes()->value(name);

        // QJsonArray inputs = data["inputs"].toArray();

        // for (int i = 0; i < inputs.count(); i++)
        // {
            // QJsonObject link_data = inputs[i].toObject();
            // QString connected_node = link_data["connected_node"].toString();
            // node *node_to_connect = _node_view->get_node(connected_node);

            // node_link *link = _node_view->get_node_link(_node, i);
            // if (link)
                // link->connect_node(node_to_connect);
        // }
    // }
    //
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
