#include <blur_node.h>

blur_node::blur_node()
{

}

blur_node::~blur_node()
{

}

QImage blur_node::render(QImage &image)
{
	for ( int x = 0; x < 1920; x++)
		for ( int y = 0; y < 1080; y++)
			QColor color = image.pixel(100, 108);

	return image;
}
