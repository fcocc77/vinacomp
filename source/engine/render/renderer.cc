#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	read = new read_node();
}

renderer::~renderer()
{

}

QImage renderer::render(int frame, QString node_name)
{
	QImage image;

	if (!project->nodes.contains(node_name))
		return image;

	node_struct *root = &project->nodes[node_name];
	QString type = root->type;


	if (type == "viewer")
	{
		QString input_node = root->inputs.value("in0").toString();
		image = render(frame, input_node);
	}

	else if (type == "read")
		image = read->render(root->params, frame);

	return image;
}
