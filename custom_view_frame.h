#ifndef CUSTOM_VIEW_FRAME_H
#define CUSTOM_VIEW_FRAME_H
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <qmath.h>
#include "frame_data.h"
#include "Frame_manager.h"
#include "frame_item.h"
class Custom_view_frame : public QGraphicsView
{
public:
    explicit Custom_view_frame(QWidget *parent = nullptr);
private:
    void gentle_zoom(double factor);
    std::shared_ptr<Frame_settings> m_frame_settings;
    std::shared_ptr<Frame_manager> m_frame_manager;
    QGraphicsScene* m_scene;
    Frame_item* m_frame_item;
    double m_zoom_factor_base;
    QPointF m_target_scene_pos, m_target_viewport_pos;
protected:
    //void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject* object, QEvent* event);

};

#endif // CUSTOM_VIEW_FRAME_H
