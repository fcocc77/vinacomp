#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	image = new QImage();

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
	QString type = node->type;

	// renderiza las entradas del nodo antes que el nodo
	QString input_node = node->inputs.value("in0").toString();
	if (!input_node.isEmpty())
		image = render(frame, input_node);
	//

	return nodes.value(node->type)->render(image, node->params, frame);
}
