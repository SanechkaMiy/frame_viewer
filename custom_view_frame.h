#ifndef CUSTOM_VIEW_FRAME_H
#define CUSTOM_VIEW_FRAME_H
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <qmath.h>
#include "custom_frame_dialog.h"
#include "frame_item.h"
#include "Frame_manager.h"
#include <QObject>
#include <QMimeData>
#include "custom_graphic_scene.h"
#include "custom_dialog_frame_settings.h"
#include <QDragEnterEvent>


class Custom_view_frame : public QGraphicsView
{
public:
    explicit Custom_view_frame(QWidget *parent = nullptr);
private:
    uint16_t m_count_frame = 0;
    Custom_dialog_frame_settings* m_dialog_frame_settings;
    std::vector<Custom_frame_dialog*> m_frame;
    Custom_graphic_scene* m_graphic_scene;
    Frame_item* m_empty_item;
    std::shared_ptr<Frame_settings> m_frame_settings;
    std::shared_ptr<Frame_manager> m_frame_manager;
    QString m_path;
protected:
    //void wheelEvent(QWheelEvent *event);

};

#endif // CUSTOM_VIEW_FRAME_H
