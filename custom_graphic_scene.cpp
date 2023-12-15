#include "custom_graphic_scene.h"

Custom_graphic_scene::Custom_graphic_scene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void Custom_graphic_scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Custom_graphic_scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls())
        foreach ( const QUrl& url, event->mimeData()->urls())
            emit send_url(url.toLocalFile());
}
