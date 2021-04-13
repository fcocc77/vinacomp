#include <project_struct.h>
#include <renderer.h>
#include <run_render.h>
#include <util.h>

void rd::write_file(cv::Mat &image, render_options opt)
{
    QString output = opt.filename;
    output.replace("###", QString::number(opt.frame));

    cv::imwrite(output.toStdString(), image,
                {cv::IMWRITE_JPEG_QUALITY, opt.jpeg_quality});
}

void rd::start_render(QString project_path, int first_frame, int last_frame)
{
    project_struct *project = new project_struct();
    project->load(project_path);

    renderer_thread *_renderer = new renderer_thread(project);

    render_options opt;
    opt.jpeg_quality = 100;
    opt.first_frame = first_frame;
    opt.last_frame = last_frame;
    opt.filename = "/home/pancho/Desktop/engine_###.jpg";

    render_data *rdata = new render_data;

    for (int i = opt.first_frame; i <= opt.last_frame; i++)
    {
        rdata->root_node = "Write1";
        rdata->frame = i;
        rdata->width = 1920;
        rdata->height = 1080;
        rdata->layer = "main";

        print("Rendering frame: " + QString::number(i) + " ....");
        _renderer->run_render(rdata);

        opt.frame = i;
        write_file(rdata->image, opt);
    }

    print("Render Completed.");
}
