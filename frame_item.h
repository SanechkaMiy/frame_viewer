#ifndef FRAME_ITEM_H
#define FRAME_ITEM_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

class Frame_item : public QGraphicsPixmapItem
{
public:
    Frame_item(QGraphicsItem *parent = nullptr);
private:
    QPointF m_start_image_position;
    QPointF m_drag_start_position;
};

#endif // FRAME_ITEM_H
