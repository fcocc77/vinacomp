#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	read = new read_node();
	blur = new blur_node();
}

renderer::~renderer()
{

}

QImage renderer::render(int frame, QString node_name)
{
	QImage image;

	if (!project->nodes.contains(node_name))
		return image;

	node_struct *node = &project->nodes[node_name];
	QString type = node->type;


	if (type == "viewer")
	{
		QString input_node = node->inputs.value("in0").toString();
		image = render(frame, input_node);
	}

	else if (type == "read")
		image = read->render(node->params, frame);

	else if (type == "blur")
	{
		QString input_node = node->inputs.value("in0").toString();
		QImage input_0_image = render(frame, input_node);

		image = blur->render(input_0_image);
	}

	return image;
}
