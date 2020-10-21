#include <panels_layout.hpp>

panels_layout::panels_layout(/* args */)
{
    this->setObjectName("panels_layout");

    splitter *_splitter = new splitter();

    QWidget *central_widget = new QWidget();
    qt::add_widget(this, _splitter);
}

panels_layout::~panels_layout()
{
}