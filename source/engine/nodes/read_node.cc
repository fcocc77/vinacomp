#include <read_node.h>

read_node::read_node()
{
	load_default_params("read");
}

read_node::~read_node()
{

}

pair <int, int> read_node::get_frame_range(QJsonObject *params) const
{
	int first = get(params, "first").toArray()[0].toInt();
	int last = get(params, "last").toArray()[0].toInt();

	return {first, last};
}

void read_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	QString file_path = get(params, "file").toString();
	QString _frame = frame_to_string(frame, 3);

	file_path.replace("###", _frame);

	(*image) = QImage(file_path).mirrored();

	frame_range =  get_frame_range(params);
}

QString read_node::frame_to_string(int frame, int frame_digits)
{
	QString _frame = "0000000000" + QString::number(frame);
	return _frame.right(frame_digits);
}
