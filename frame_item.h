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

};

#endif // FRAME_ITEM_H
