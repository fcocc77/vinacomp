#include <renderer.h>

renderer::renderer()
{

}

renderer::~renderer()
{

}

void renderer::render(int frame)
{
	print("render:" + QString::number(frame));
}
