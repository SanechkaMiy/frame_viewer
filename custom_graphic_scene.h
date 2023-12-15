#ifndef CUSTOM_GRAPHIC_SCENE_H
#define CUSTOM_GRAPHIC_SCENE_H
#include <QApplication>
#include <QDragEnterEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMimeData>
#include <QUrl>
#include <iostream>
class Custom_graphic_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Custom_graphic_scene(QObject *parent = nullptr);
signals:
    void send_url(QString);
protected:
    QGraphicsRectItem* background = nullptr;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
};

#endif // CUSTOM_GRAPHIC_SCENE_H
