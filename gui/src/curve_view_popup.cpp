#include <curve_view.hpp>

void curve_view::popup_setup_ui()
{
    popup = new QMenu(this);

    // Edit : Select All Key Frames
    action *select_all_action = new action("Select All Keyframes", "Ctrl+A", "select_all_a");
    select_all_action->connect_to(this, [this]() {
        select_all_key_frames();
    });
    popup->addAction(select_all_action);
    //
    //

    // Edit : Delete Key Frames
    action *delete_keyframes_action = new action("Delete Keyframes", "Backspace", "close_a");
    popup->addAction(delete_keyframes_action);
    //
    //

    popup->addSeparator();

    // Interpolation_menu : Linear
    action *linear_action = new action("Linear Interpolation", "L", "linear_a");
    popup->addAction(linear_action);
    //
    //

    // Interpolation_menu : Horizontal
    action *horizontal_action = new action("Horizontal Interpolation", "H", "curve_horizontal_a");
    popup->addAction(horizontal_action);
    //
    //

    // Interpolation_menu : Smooth
    action *smooth_action = new action("Smooth Interpolation", "Z", "curve_smooth_a");
    popup->addAction(smooth_action);
    //
    //

    // Interpolation_menu : Break
    action *break_action = new action("Break", "X", "curve_break_a");
    popup->addAction(break_action);
    //
    //

    popup->addSeparator();

    // View : Center To Curve
    action *center_action = new action("Center To Curve", "F", "fit_curve_a");
    center_action->connect_to(this, [this]() {
        fit_viewport_to_keyframes();
    });
    popup->addAction(center_action);
    //
    //
}

void curve_view::popup_press(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        popup->popup(QCursor::pos());
        popup->show();
    }
}