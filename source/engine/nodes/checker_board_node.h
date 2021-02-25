#ifndef CHECKER_BOARD_NODE_H
#define CHECKER_BOARD_NODE_H

#include <QPainter>
#include <node_engine.h>

class checker_board_node : public node_engine
{
public:
	checker_board_node();
	~checker_board_node();

	void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // CHECKER_BOARD_NODE_H
