#include <tool_bar.hpp>

tool_bar::tool_bar(/* args */)
{
    this->setObjectName("tool_bar");
    this->setMaximumWidth(40);

    layout = new QVBoxLayout();
    layout->setContentsMargins(2, 10, 2, 10);
    this->setLayout(layout);

    setup_ui();
}

tool_bar::~tool_bar()
{
}

void tool_bar::setup_ui()
{

    int icon_size = 25;

    {
        QMenu *image_menu = new QMenu(this);

        QAction *checker_board_action = new QAction("CheckerBoard");
        checker_board_action->setIcon(QIcon("resources/images/checker_a.png"));
        image_menu->addAction(checker_board_action);

        QAction *constant_action = new QAction("Constant");
        constant_action->setIcon(QIcon("resources/images/constant_a.png"));
        image_menu->addAction(constant_action);

        QAction *read_action = new QAction("Read");
        read_action->setIcon(QIcon("resources/images/read_a.png"));
        image_menu->addAction(read_action);

        QAction *write_action = new QAction("Write");
        write_action->setIcon(QIcon("resources/images/save_a.png"));
        image_menu->addAction(write_action);

        QAction *viewer_action = new QAction("Viewer");
        viewer_action->setIcon(QIcon("resources/images/viewer_a.png"));
        image_menu->addAction(viewer_action);

        QPushButton *image = new QPushButton();
        connect(image, &QPushButton::clicked, this, [=]() {
            image_menu->popup(QCursor::pos());
            image_menu->show();
        });
        qt::set_icon(image, "image_a", icon_size);
        layout->addWidget(image);
    }
    {
        QMenu *draw_menu = new QMenu(this);

        QAction *noise_action = new QAction("Noise");
        noise_action->setIcon(QIcon("resources/images/noise_a.png"));
        draw_menu->addAction(noise_action);

        QAction *radial_action = new QAction("Radial");
        radial_action->setIcon(QIcon("resources/images/radial_a.png"));
        draw_menu->addAction(radial_action);

        QAction *ramp_action = new QAction("Ramp");
        ramp_action->setIcon(QIcon("resources/images/ramp_a.png"));
        draw_menu->addAction(ramp_action);

        QAction *grid_action = new QAction("Grid");
        grid_action->setIcon(QIcon("resources/images/grid_a.png"));
        draw_menu->addAction(grid_action);

        QAction *rectangle_action = new QAction("Rectangle");
        rectangle_action->setIcon(QIcon("resources/images/constant_a.png"));
        draw_menu->addAction(rectangle_action);

        QAction *text_action = new QAction("Text");
        text_action->setIcon(QIcon("resources/images/text_a.png"));
        draw_menu->addAction(text_action);

        QPushButton *draw = new QPushButton();
        connect(draw, &QPushButton::clicked, this, [=]() {
            draw_menu->popup(QCursor::pos());
            draw_menu->show();
        });
        qt::set_icon(draw, "brush_a", icon_size);
        layout->addWidget(draw);
    }
    {
        QMenu *time_menu = new QMenu(this);

        QAction *frame_hold_action = new QAction("FrameHold");
        frame_hold_action->setIcon(QIcon("resources/images/frame_hold_a.png"));
        time_menu->addAction(frame_hold_action);

        QAction *frame_range_action = new QAction("FrameRange");
        frame_range_action->setIcon(QIcon("resources/images/frame_range_a.png"));
        time_menu->addAction(frame_range_action);

        QAction *time_offset_action = new QAction("TimeOffset");
        time_offset_action->setIcon(QIcon("resources/images/time_offset_a.png"));
        time_menu->addAction(time_offset_action);

        QPushButton *time = new QPushButton();
        connect(time, &QPushButton::clicked, this, [=]() {
            time_menu->popup(QCursor::pos());
            time_menu->show();
        });
        qt::set_icon(time, "time_a", icon_size);
        layout->addWidget(time);
    }
    {

        QMenu *channel_menu = new QMenu(this);

        QAction *shuffle_action = new QAction("Shuffle");
        shuffle_action->setIcon(QIcon("resources/images/shuffle_a.png"));
        channel_menu->addAction(shuffle_action);

        QAction *copy_channel_action = new QAction("Copy");
        copy_channel_action->setIcon(QIcon("resources/images/copy_channel_a.png"));
        channel_menu->addAction(copy_channel_action);

        QPushButton *channel = new QPushButton();
        connect(channel, &QPushButton::clicked, this, [=]() {
            channel_menu->popup(QCursor::pos());
            channel_menu->show();
        });
        qt::set_icon(channel, "channel_a", icon_size);
        layout->addWidget(channel);
    }
    {
        QMenu *color_menu = new QMenu(this);

        QAction *saturation_action = new QAction("Saturation");
        saturation_action->setIcon(QIcon("resources/images/saturation_a.png"));
        color_menu->addAction(saturation_action);

        QAction *clamp_action = new QAction("Clamp");
        clamp_action->setIcon(QIcon("resources/images/clamp_a.png"));
        color_menu->addAction(clamp_action);

        QAction *invert_action = new QAction("Invert");
        invert_action->setIcon(QIcon("resources/images/invert_a.png"));
        color_menu->addAction(invert_action);

        QAction *grade_action = new QAction("Grade");
        grade_action->setIcon(QIcon("resources/images/grade_a.png"));
        color_menu->addAction(grade_action);

        QPushButton *color = new QPushButton();
        connect(color, &QPushButton::clicked, this, [=]() {
            color_menu->popup(QCursor::pos());
            color_menu->show();
        });
        qt::set_icon(color, "color_a", icon_size);
        layout->addWidget(color);
    }
    {

        QMenu *filter_menu = new QMenu(this);

        QAction *blur_action = new QAction("Blur");
        blur_action->setIcon(QIcon("resources/images/noise_a.png"));
        filter_menu->addAction(blur_action);

        QAction *edge_detect_action = new QAction("EdgeDetect");
        edge_detect_action->setIcon(QIcon("resources/images/edge_detect_a.png"));
        filter_menu->addAction(edge_detect_action);

        QAction *erode_action = new QAction("Erode");
        erode_action->setIcon(QIcon("resources/images/erode_a.png"));
        filter_menu->addAction(erode_action);

        QAction *sharpen_action = new QAction("Sharpen");
        sharpen_action->setIcon(QIcon("resources/images/sharpen_a.png"));
        filter_menu->addAction(sharpen_action);

        QPushButton *filter = new QPushButton();
        connect(filter, &QPushButton::clicked, this, [=]() {
            filter_menu->popup(QCursor::pos());
            filter_menu->show();
        });
        qt::set_icon(filter, "filter_a", icon_size);
        layout->addWidget(filter);
    }
    {

        QMenu *keyer_menu = new QMenu(this);

        QAction *keyer_action = new QAction("Keyer");
        keyer_action->setIcon(QIcon("resources/images/keyer_a.png"));
        keyer_menu->addAction(keyer_action);

        QPushButton *keyer = new QPushButton();
        connect(keyer, &QPushButton::clicked, this, [=]() {
            keyer_menu->popup(QCursor::pos());
            keyer_menu->show();
        });
        qt::set_icon(keyer, "keyer_a", icon_size);
        layout->addWidget(keyer);
    }
    {

        QMenu *merge_menu = new QMenu(this);

        QAction *merge_action = new QAction("Merge");
        merge_action->setIcon(QIcon("resources/images/merge_a.png"));
        merge_menu->addAction(merge_action);

        QAction *premult_action = new QAction("Premult");
        premult_action->setIcon(QIcon("resources/images/premult_a.png"));
        merge_menu->addAction(premult_action);

        QAction *unpremult_action = new QAction("Unpremult");
        unpremult_action->setIcon(QIcon("resources/images/unpremult_a.png"));
        merge_menu->addAction(unpremult_action);

        QAction *dissolve_action = new QAction("Dissolve");
        dissolve_action->setIcon(QIcon("resources/images/dissolve_a.png"));
        merge_menu->addAction(dissolve_action);

        QAction *switch_action = new QAction("Switch");
        switch_action->setIcon(QIcon("resources/images/switch_a.png"));
        merge_menu->addAction(switch_action);

        QPushButton *merge = new QPushButton();
        connect(merge, &QPushButton::clicked, this, [=]() {
            merge_menu->popup(QCursor::pos());
            merge_menu->show();
        });
        qt::set_icon(merge, "merge_a", icon_size);
        layout->addWidget(merge);
    }
    {

        QMenu *transform_menu = new QMenu(this);

        QAction *transform_action = new QAction("Transform");
        transform_action->setIcon(QIcon("resources/images/position_a.png"));
        transform_menu->addAction(transform_action);

        QAction *bbox_adjust_action = new QAction("BBoxAdjust");
        bbox_adjust_action->setIcon(QIcon("resources/images/bbox_adjust_a.png"));
        transform_menu->addAction(bbox_adjust_action);

        QAction *cornel_pin_action = new QAction("CornelPin");
        cornel_pin_action->setIcon(QIcon("resources/images/cornel_pin_a.png"));
        transform_menu->addAction(cornel_pin_action);

        QAction *crop_action = new QAction("Crop");
        crop_action->setIcon(QIcon("resources/images/crop_a.png"));
        transform_menu->addAction(crop_action);

        QAction *reformat_action = new QAction("Reformat");
        reformat_action->setIcon(QIcon("resources/images/reformat_a.png"));
        transform_menu->addAction(reformat_action);

        QAction *position_action = new QAction("Position");
        position_action->setIcon(QIcon("resources/images/position_a.png"));
        transform_menu->addAction(position_action);

        QAction *idistort_action = new QAction("IDistort");
        idistort_action->setIcon(QIcon("resources/images/idistort_a.png"));
        transform_menu->addAction(idistort_action);

        QAction *mirror_action = new QAction("Mirror");
        mirror_action->setIcon(QIcon("resources/images/mirror_a.png"));
        transform_menu->addAction(mirror_action);

        QAction *lens_distortion_action = new QAction("LensDistortion");
        lens_distortion_action->setIcon(QIcon("resources/images/lens_distortion_a.png"));
        transform_menu->addAction(lens_distortion_action);

        QPushButton *transform = new QPushButton();
        connect(transform, &QPushButton::clicked, this, [=]() {
            transform_menu->popup(QCursor::pos());
            transform_menu->show();
        });
        qt::set_icon(transform, "transform_a", icon_size);
        layout->addWidget(transform);
    }
    {
        QMenu *other_menu = new QMenu(this);

        QAction *backdrop_action = new QAction("Backdrop");
        backdrop_action->setIcon(QIcon("resources/images/backdrop_a.png"));
        other_menu->addAction(backdrop_action);

        QAction *dot_action = new QAction("Dot");
        dot_action->setIcon(QIcon("resources/images/dot_a.png"));
        other_menu->addAction(dot_action);

        QPushButton *other = new QPushButton();
        connect(other, &QPushButton::clicked, this, [=]() {
            other_menu->popup(QCursor::pos());
            other_menu->show();
        });
        qt::set_icon(other, "other_a", icon_size);
        layout->addWidget(other);
    }
    layout->addStretch();

    QPushButton *script_layout = new QPushButton();
    qt::set_icon(script_layout, "view_compact_a", icon_size);
    layout->addWidget(script_layout);

    QPushButton *comp_layout = new QPushButton();
    qt::set_icon(comp_layout, "vertical_split_a", icon_size);
    layout->addWidget(comp_layout);
}