#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	image = new QImage();

	// los nodos que se definen globalmente aparte de insertarlos a la lista 'nodes'
	// es porque tienen funciones aparte de 'render'
	time_offset = new time_offset_node();
	nodes.insert("time_offset", time_offset);
	//

	nodes.insert("viewer", new viewer_node());
	nodes.insert("read", new read_node());
	nodes.insert("blur", new blur_node());
}

renderer::~renderer()
{

}

QImage *renderer::render(int frame, QString node_name)
{
	if (!project->nodes.contains(node_name))
		return image;

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
		image = render(frame, input_node);
	//

	// renderiza el nodo actual
	if (_node_engine)
		image = _node_engine->render(image, node->params, frame);
	//

	return image;
}
