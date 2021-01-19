#include <renderer.h>

renderer::renderer(project_struct *_project)
	: project(_project)
{
	read = new read_node();
}

renderer::~renderer()
{

}

QImage renderer::render(int frame)
{
	QJsonObject *params = project->nodes["Read1"].params;
	return read->render(params);
}
