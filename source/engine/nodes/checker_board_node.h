#ifndef CHECKER_BOARD_NODE_H
#define CHECKER_BOARD_NODE_H

#include <QPainter>
#include <node_engine.h>

class checker_board_node : public node_engine
{
public:
	checker_board_node();
	~checker_board_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // CHECKER_BOARD_NODE_H
