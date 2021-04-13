#include <QString>
#include <engine.h>
#include <run_render.h>

int main(int argc, char *argv[])
{

    // QString input_py = argv[1];
    // engine _engine(input_py);

    QString project_path = argv[1];
    QString first_frame = argv[2];
    QString last_frame = argv[3];

    rd::start_render(project_path, first_frame.toInt(), last_frame.toInt());
}
