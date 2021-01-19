#include <read_node.h>

read_node::read_node()
{

}

read_node::~read_node()
{

}

QImage read_node::render(QJsonObject *params)
{
	QString file_name = params->value("file").toString();

	QImage image = QImage(file_name);
	image = image.mirrored();

	return image;
}
