#include <curve_editor.hpp>

curve_editor ::curve_editor(/* args */)
{
    this->setObjectName("curve_editor");

    setup_ui();
}

curve_editor ::~curve_editor()
{
}

void curve_editor::setup_ui()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QTreeWidget *knobs_tree = knobs_tree_setup_ui();

    layout->addWidget(knobs_tree);

    curve_view *view = new curve_view();
    view->setObjectName("graphics_view");
    layout->addWidget(view);

    // create a scene and add it your view
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    view->setScene(scene);

    //
    //
}

QTreeWidget *curve_editor::knobs_tree_setup_ui()
{
    QTreeWidget *tree = new QTreeWidget();
    tree->setObjectName("knobs_tree");
    tree->setMaximumWidth(300);
    tree->setMinimumWidth(300);

    //
    //

    return tree;
}

curve_view::curve_view(/* args */)
{
}

curve_view::~curve_view()
{
}

void curve_view::drawBackground(QPainter *painter, const QRectF &rect)
{
    int mGridSize = 20;

    qreal left = int(rect.left()) - (int(rect.left()) % mGridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % mGridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += mGridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += mGridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    QVarLengthArray<QLineF, 100> thickLines;

    for (qreal x = left; x < rect.right(); x += mGridSize * 5)
        thickLines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += mGridSize * 5)
        thickLines.append(QLineF(rect.left(), y, rect.right(), y));

    QPen myPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(55, 55, 55, 255)));
    painter->setPen(myPen);
    painter->drawRect(rect);

    QPen penHLines(QColor(75, 75, 75), 1, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(penHLines);
    painter->drawLines(lines.data(), lines.size());

    painter->setPen(QPen(QColor(100, 100, 100), 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    painter->drawLines(thickLines.data(), thickLines.size());

    painter->setPen(Qt::blue);

    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += mGridSize)
    {
        for (qreal y = top; y < rect.bottom(); y += mGridSize)
        {
            points.append(QPointF(x, y));
        }
    }
    painter->drawPoints(points.data(), points.size());
}
