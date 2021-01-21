#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	// los nodos que se definen globalmente aparte de insertarlos a la lista 'nodes'
	// es porque tienen funciones aparte de 'render'
	time_offset = new time_offset_node();
	nodes.insert("time_offset", time_offset);
	//

	nodes.insert("viewer", new viewer_node());
	nodes.insert("read", new read_node());
	nodes.insert("blur", new blur_node());
	nodes.insert("frame_range", new frame_range_node());
}

renderer::~renderer()
{

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

	// los nodos de tiempo tienen que modificar todos los nodos entrantes
	// por eso estos nodos tienen que ir antes de renderizar las entradas
	if (node->type == "time_offset")
		time_offset->set_offset(node->params, frame);
	//

	// renderiza las entradas del nodo antes que el nodo
	QString input_node = node->inputs.value("in0").toString();
	if (!input_node.isEmpty())
		render(image, frame, input_node, frame_range, bbox);
	//

	// renderiza el nodo actual
	if (_node_engine)
		_node_engine->render(image, node->params, frame, frame_range, bbox);
	//
}
