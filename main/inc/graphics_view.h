#ifndef __GRAPHICS_VIEW_H__
#define __GRAPHICS_VIEW_H__

#include <ec_core.h>


class GraphicsView : public QGraphicsView {

protected:

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *event);

};

#endif
