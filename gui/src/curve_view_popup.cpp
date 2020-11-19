#include <curve_view.hpp>

void curve_view::popup_setup_ui()
{
    popup = new QMenu(this);

    // Edit : Select All Key Frames
    QAction *select_all_action = new QAction("Select All Keyframes");
    select_all_action->setIcon(QIcon("resources/images/select_all_a.png"));
    select_all_action->setShortcut(QString("Ctrl+A"));
    connect(select_all_action, &QAction::triggered, this, &curve_view::select_all_key_frames);
    popup->addAction(select_all_action);
    //
    //

    // Edit : Delete Key Frames
    QAction *delete_keyframes_action = new QAction("Delete Keyframes");
    delete_keyframes_action->setIcon(QIcon("resources/images/close_a.png"));
    delete_keyframes_action->setShortcut(QString("Backspace"));
    popup->addAction(delete_keyframes_action);
    //
    //

    popup->addSeparator();

    // Interpolation_menu : Linear
    QAction *linear_action = new QAction("Linear Interpolation");
    linear_action->setIcon(QIcon("resources/images/linear_a.png"));
    linear_action->setShortcut(QString("L"));
    connect(linear_action, &QAction::triggered, this, [this]() {
    });
    popup->addAction(linear_action);
    //
    //

    // Interpolation_menu : Horizontal
    QAction *horizontal_action = new QAction("Horizontal Interpolation");
    horizontal_action->setIcon(QIcon("resources/images/curve_horizontal_a.png"));
    horizontal_action->setShortcut(QString("H"));
    connect(horizontal_action, &QAction::triggered, this, [this]() {
    });
    popup->addAction(horizontal_action);
    //
    //

    // Interpolation_menu : Smooth
    QAction *smooth_action = new QAction("Smooth Interpolation");
    smooth_action->setIcon(QIcon("resources/images/curve_smooth_a.png"));
    smooth_action->setShortcut(QString("Z"));
    connect(smooth_action, &QAction::triggered, this, [this]() {
    });
    popup->addAction(smooth_action);
    //
    //

    // Interpolation_menu : Break
    QAction *break_action = new QAction("Break");
    break_action->setIcon(QIcon("resources/images/curve_break_a.png"));
    break_action->setShortcut(QString("X"));
    connect(break_action, &QAction::triggered, this, [this]() {
    });
    popup->addAction(break_action);
    //
    //

    popup->addSeparator();

    // View : Center To Curve
    QAction *center_action = new QAction("Center To Curve");
    center_action->setIcon(QIcon("resources/images/fit_curve_a.png"));
    center_action->setShortcut(QString("F"));
    connect(center_action, &QAction::triggered, this, &curve_view::fit_viewport_to_keyframes);
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