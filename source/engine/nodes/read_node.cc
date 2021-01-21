#include <read_node.h>

read_node::read_node()
{
	load_default_params("read");
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
	QString file_path = get("file", params).toString();
	int first = get("first", params).toArray()[0].toInt();
	int last = get("last", params).toArray()[0].toInt();

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
