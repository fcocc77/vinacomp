#include <viewer.h>

viewer::viewer()
{
    _viewer_gl = new viewer_gl();
    _viewer_gl->setObjectName("viewer_graphics");

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);
    this->setObjectName("viewer");

    setup_ui();
    
}

viewer::~viewer()
{
}
