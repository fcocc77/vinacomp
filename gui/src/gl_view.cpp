#include <gl_view.hpp>

gl_view::gl_view(/* args */)
{
    zoomScale = 1;
}

gl_view::~gl_view()
{
}

void gl_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, .1, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);

    format().setSamples(10);
}

void gl_view::paintGL()
{
    zoom();
}

void gl_view::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void gl_view::zoom()
{

    int range = 1000;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(height()) / width();

    float move = cursor_x * zoomScale * 0.5;

    print(move);

    glTranslatef(-move, 0.0, 0);
    glScaled(zoomScale * aspect, zoomScale, 1);

    glMatrixMode(GL_MODELVIEW);
}

QPointF gl_view::map_position(QPoint mouse_position)
{
    // Calcula la posicion del mouse a la posicion de la escena OpenGl
    float zoom_val = 1.0;
    QList<float> ortho_2d = {-zoom_val, +zoom_val, -zoom_val, +zoom_val};

    // Primero, calcular las coordenadas "normalizadas" del mouse dividiendo por tamaño
    float mouse_norm_x = float(mouse_position.x()) / size().width();
    float mouse_norm_y = float(mouse_position.y()) / size().height();

    // Mapear coordenadas al rango de proyección ortográfica
    float mouse_ortho_x = (mouse_norm_x * (ortho_2d[1] - ortho_2d[0])) + ortho_2d[0];
    float mouse_ortho_y = (mouse_norm_y * (ortho_2d[3] - ortho_2d[2])) + ortho_2d[2];

    return {mouse_ortho_x, -mouse_ortho_y};
}

void gl_view::wheelEvent(QWheelEvent *event)
{
    print("well");

    QPoint numDegrees = event->angleDelta();
    if (numDegrees.y() < 0)
        zoomScale = zoomScale / 1.1;
    else
        zoomScale = zoomScale * 1.1;

    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    float x1_percent = x_pos * 100 / this->width();
    float x2_percent = 100 - x1_percent;

    x1_range = x1_percent;
    x2_range = x2_percent;

    // float last_cursor_x = cursor_x;
    cursor_x = 2.0f * (x_pos + 0.5) / this->width() - 1.0;
    // cursor_x -= last_cursor_x;
    cursor_y = 2.0f * (y_pos + 0.5) / this->height() - 1.0;

    update();
}
