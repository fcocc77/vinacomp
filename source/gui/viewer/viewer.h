#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Gui
#include <combo_box.h>
#include <time_line.h>
#include <viewer_gl.h>
#include <vinacomp_namespace.h>

// Engine
#include <project_struct.h>
#include <render_data.h>
#include <renderer.h>

class viewer : public QWidget
{
private:
    QVBoxLayout *layout;
    project_struct *project;

    render_data *rdata;

    viewer_gl *_viewer_gl;
    time_line *_time_line;
    QString playing_option;
    QString name;
    QMenu *menu;

    QWidget *_vinacomp;
    renderer *_renderer;

    bool global_range;
    QString input_range_way;
    int first_frame, last_frame;
    int input, output;
    float frame_rate;
    bool in_out;
    bool render_pause;
    int visible_channel;
    QString current_layer;
    bool playing;

    float gain, gamma;

    //
    void setup_ui();
    void create_menu();
    QWidget *player_setup_ui();
    QWidget *info_setup_ui();
    QWidget *control_setup_ui();
    QWidget *image_correction_setup_ui();
    //
    //

    // Acciones Control
    action *pause_action;
    action *out_frame_action;
    action *handler_snap_action;
    action *render_area_action;
    action *proxy_action;
    action *multi_lines_action;
    action *refresh_action;

    // Acciones Player
    action *skip_forward_action;
    action *skip_backward_action;
    action *next_key_frame_action;
    action *previous_key_frame_action;
    action *play_forward_action;
    action *stop_forward_action;
    action *play_backward_action;
    action *stop_backward_action;
    action *visibility_in_out_action;
    action *input_action;
    action *output_action;
    //

    // General
    void set_frame(int frame);
    void set_frame_rate(float rate);
    void enable_in_out(bool enable);
    void set_in_out(int input, int output);
    //
    // Player
    QLineEdit *frame_edit;
    QLineEdit *skip_frame_edit;
    QLineEdit *input_frame_edit;
    QLineEdit *output_frame_edit;
    combo_box *frame_rate_menu;
    combo_box *play_back_options;
    combo_box *range_way_menu;
    int play_direction;
    int last_time;
    float frame_rate_sum;
    int frame_rate_count;
    void calculate_frame_rate();
    void player_init();
    void play(int direction);
    void playing_now();
    void set_playing_option(QString option);
    void stop();
    void go_to_first_frame();
    void go_to_last_frame();
    void next_frame();
    void previous_frame();
    void next_key_frame();
    void previous_key_frame();
    void skip_forward();
    void skip_backward();
    pair<int, int> get_current_range() const;
    //

    // Controls
    combo_box *display_channel;
    void change_channel(int index);
    void change_layer(QString layer);
    void purge_ram();
    void set_handler_snap(bool enable);
    void set_outside_pixels(bool enable);
    void set_render_area(bool enable);
    void set_proxy(bool enable);
    void set_multi_lines(bool enable);

    void resizeEvent(QResizeEvent *event) override;

public:
    viewer(QString _name, project_struct *_project, renderer *_renderer,
           QWidget *_vinacomp);
    ~viewer();

    inline void set_frame_range(int _first_frame, int _last_frame);
    inline QString get_name() const;
    inline viewer_gl *get_viewer_gl() const;
    inline time_line *get_time_line() const;
    void update_render(bool claer_init_image = false);
    void finished_render(render_data rdata);
    void update_input_range();
};

inline time_line *viewer::get_time_line() const
{
    return _time_line;
}

inline QString viewer::get_name() const
{
    return name;
}

inline void viewer::set_frame_rate(float rate)
{
    frame_rate = rate;
}

inline viewer_gl *viewer::get_viewer_gl() const
{
    return _viewer_gl;
}

#endif // VIEWER_H
