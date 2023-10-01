#include "custom_view_frame.h"
#include <QDebug>

Custom_view_frame::Custom_view_frame(QWidget *parent) :
    QGraphicsView(parent)
{
    m_frame_settings.reset(new Frame_settings());
    m_frame_manager.reset(new Frame_manager(m_frame_settings));
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    m_frame_item = new Frame_item();
    m_scene->addItem(m_frame_item);
    m_frame_manager->load_image();
    m_frame_item->setPixmap(m_frame_manager->add_image());
    //m_frame_item->setFlags(QGraphicsItem::ItemIsMovable);
    //m_frame_item->setScale(0.25);
    m_zoom_factor_base = 1.0015;
    viewport()->installEventFilter(this);
    setMouseTracking(true);
}

void Custom_view_frame::gentle_zoom(double factor)
{
    scale(factor, factor);
    centerOn(m_target_scene_pos);
    QPointF delta_viewport_pos = m_target_viewport_pos - QPointF(viewport()->width() / 2.0,
                                                                 viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(m_target_scene_pos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));
    //qDebug() << ;
}

bool Custom_view_frame::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = m_target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            m_target_viewport_pos = mouse_event->pos();
            m_target_scene_pos = mapToScene(mouse_event->pos());
            qDebug() << mapToScene(m_frame_item->boundingRect().topLeft().toPoint()) << mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()) << m_target_scene_pos;
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        setDragMode(ScrollHandDrag);
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == Qt::NoModifier) {
            if (wheel_event->orientation() == Qt::Vertical) {
                double angle = wheel_event->angleDelta().y();
                double factor = qPow(m_zoom_factor_base, angle);
                gentle_zoom(factor);
                return true;
            }
        }
    }
    Q_UNUSED(object)
    return false;
}


