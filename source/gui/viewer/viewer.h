#ifndef VIEWER_H
#define VIEWER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QTimeLine>
#include <QWidget>

// Gui
#include <action.h>
#include <combo_box.h>
#include <knob_floating.h>
#include <qt.h>
#include <slider.h>
#include <time_line.h>
#include <tools.h>
#include <util.h>
#include <viewer_gl.h>
#include <new_layers.h>
#include <global.h>

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
    new_layers *_new_layers;
    QString playing_option;
    QString name;
    QMenu *menu;

    QWidget *_vinacomp;
    renderer *_renderer;
    global *glob;

    bool global_range;
    QString input_range_way;
    int first_frame, last_frame;
    int input, output;
    int current_frame;
    float frame_rate;
    bool in_out;
    bool render_pause;
    int visible_channel;
    QString current_layer;

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
    QTimeLine *qtime_line;
    void player_init();
    void play(QTimeLine::Direction direction);
    void play_finished();
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
    void new_layer_widget(combo_box *cb);

public:
    viewer(QString _name, project_struct *_project, renderer *_renderer,
           global *glob, QWidget *_vinacomp);
    ~viewer();

    void set_frame_range(int _first_frame, int _last_frame);
    QString get_name() const;
    void update_render();
    void update_input_range();
    viewer_gl *get_viewer_gl() const;
};

#endif // VIEWER_H
