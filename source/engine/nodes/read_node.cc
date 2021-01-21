#include <read_node.h>

read_node::read_node()
{

}

read_node::~read_node()
{

}

void read_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	QString file_path = params->value("file").toString();
	int first = params->value("first").toArray()[0].toInt();
	int last = params->value("last").toArray()[0].toInt();

	QString _frame = frame_to_string(frame, 3);

	file_path.replace("###", _frame);

	(*image) = QImage(file_path).mirrored();

	frame_range = {first, last};
}

QString read_node::frame_to_string(int frame, int frame_digits)
{
	QString _frame = "0000000000" + QString::number(frame);
	return _frame.right(frame_digits);
}
