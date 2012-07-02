#include "../inc/graphics_view.h"


QSize GraphicsView::minimumSizeHint() const {

    return QSize(500, 500);
}

QSize GraphicsView::sizeHint() const {

    return QSize(800, 800);
}

void GraphicsView::resizeEvent(QResizeEvent *event) {

    if(scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));

    QGraphicsView::resizeEvent(event);
}
