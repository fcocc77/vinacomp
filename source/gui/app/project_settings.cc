#include <project_settings.h>


project_settings::project_settings()
{
	this->hide();
	this->setMinimumWidth(500);
	this->setMaximumWidth(500);

	layout = new QVBoxLayout(this);
	QTreeWidget *tree = new QTreeWidget();
	layout->addWidget(tree);

}

project_settings::~project_settings()
{

}
