#ifndef RUN_RENDER_H
#define RUN_RENDER_H

#include <QString>
#include <opencv2/opencv.hpp>

struct render_options
{
    int jpeg_quality;
    int first_frame;
    int last_frame;
    QString filename;
    int frame;
};

namespace rd
{
void write_file(cv::Mat &image, render_options opt);
void start_render(QString project_path, int first_frame, int last_frame);
} // namespace rd

#endif // RUN_RENDER_H
