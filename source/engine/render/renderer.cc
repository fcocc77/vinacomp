#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	// los nodos que se definen globalmente aparte de insertarlos a la lista 'nodes'
	// es porque tienen funciones aparte de 'render'
	time_offset = new time_offset_node();
	read = new read_node();
	_frame_range = new frame_range_node();

	nodes.insert("time_offset", time_offset);
	nodes.insert("read", read);
	nodes.insert("frame_range", _frame_range);
	//

	nodes.insert("viewer", new viewer_node());
	nodes.insert("blur", new blur_node());
	nodes.insert("position", new position_node());
	nodes.insert("invert", new invert_node());
	nodes.insert("reformat", new reformat_node());
	nodes.insert("constant", new constant_node());
	nodes.insert("ramp", new ramp_node());
	nodes.insert("radial", new radial_node());
	nodes.insert("noise", new noise_node());
	nodes.insert("shuffle", new shuffle_node());
	nodes.insert("grid", new grid_node());
	nodes.insert("checker_board", new checker_board_node());
}

renderer::~renderer()
{

}

QString renderer::get_input_node(QString node_name, int input) const
{
	node_struct *node = &project->nodes[node_name];
	QString input_node = node->inputs.value("in0").toString();

	return input_node;
}

pair <int, int> renderer::get_frame_range(QString node_name) const
{
	// calcula el 'frame range' de un nodo, sin renderizar el nodo,
	// el 'frame range' se encuentra solo en algunos nodos como
	// read, frame_range, retime, y todo los nodos que tengan 'frame_range'
	node_struct *node = &project->nodes[node_name];

	if (node->type == "read")
		return read->get_frame_range(node->params);
	else if (node->type == "frame_range")
		return _frame_range->get_frame_range(node->params);

	QString input_node = node->inputs.value("in0").toString();
	if (!input_node.isEmpty())
		return get_frame_range(input_node);

	return {};
}

void renderer::render(
		QImage *image,
		int frame, 
		QString node_name,
		pair<int, int> &frame_range,
		QRect &bbox
		)
{
	if (!project->nodes.contains(node_name))
		return;

	node_struct *node = &project->nodes[node_name];
	node_engine *_node_engine = nodes.value(node->type);

	bool disable = false;
	if (_node_engine)
		disable = _node_engine->get(node->params, "disable_node").toBool();

	// los nodos de tiempo tienen que modificar todos los nodos entrantes
	// por eso estos nodos tienen que ir antes de renderizar las entradas
	if (node->type == "time_offset" && !disable)
		time_offset->set_offset(node->params, frame, node_name, this);
	//

	// renderiza las entradas del nodo antes que el nodo
	QString input_node = node->inputs.value("in0").toString();
	if (!input_node.isEmpty())
		render(image, frame, input_node, frame_range, bbox);
	//

	// renderiza el nodo actual
	if (_node_engine && !disable)
		_node_engine->render(image, node->params, frame, frame_range, bbox);
	//
}
