#include "custom_view_frame.h"
#include <QDebug>

Custom_view_frame::Custom_view_frame(QWidget *parent) :
    QGraphicsView(parent)
{
    m_frame_settings.reset(new Frame_settings());
    m_frame_manager.reset(new Frame_manager(m_frame_settings));
    m_graphic_scene = new Custom_graphic_scene(this);
    setScene(m_graphic_scene);
    m_frame_item = new Frame_item();
    m_graphic_scene->addItem(m_frame_item);
    m_frame_item->setAcceptDrops(true);
    //m_frame_manager->load_image("path");
    //m_frame_item->setPixmap(m_frame_manager->add_image(m_rows, m_colls));
    m_frame_item->setPixmap(QPixmap(128, 128));
    scale(15, 15);
    connect(m_graphic_scene, &Custom_graphic_scene::send_url, this, [this](QString path)
    {
        //m_frame_item->setPixmap(QPixmap(m_rows, m_colls));

        m_path = path;
        m_dialog_frame_settings = new Custom_dialog_frame_settings(this);
        m_dialog_frame_settings->setModal(true);
        m_dialog_frame_settings->exec();
        m_frame_manager->load_image(m_path, m_rows, m_colls);
        m_frame_item->setPixmap(m_frame_manager->add_image(m_rows, m_colls));
        scale(0.1, 0.1);
        m_is_image_here = true;
    });

    //m_frame_item->setFlags(QGraphicsItem::ItemIsMovable);
    //m_frame_item->setScale(2);
    m_zoom_factor_base = 1.0015;
    viewport()->installEventFilter(this);
    setMouseTracking(true);
}

void Custom_view_frame::load_image_from_path(uint16_t rows, uint16_t colls)
{

    m_frame_manager->load_image(m_path, rows, colls);
    m_frame_item->setPixmap(m_frame_manager->add_image(rows, colls));
    m_is_image_here = true;
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

void Custom_view_frame::clear_text_item()
{
    if (m_text_item.size() != 0)
    {
        for (auto el : m_text_item)
            delete el;
        m_text_item.clear();
    }
}

void Custom_view_frame::add_text_item_on_pixmap(Text_rect &text_rect)
{
    clear_text_item();
    for (uint16_t coll = text_rect.m_pos_x; coll < text_rect.m_width; ++coll)
    {
        for (uint16_t row = text_rect.m_pos_y; row < text_rect.m_height; ++row)
        {
            create_text_item(text_rect, coll, row);
        }
    }
}

void Custom_view_frame::create_text_item(Text_rect &text_rect, uint16_t &coll, uint16_t &row)
{
    m_text_item.push_back(new QGraphicsSimpleTextItem(QString::number(m_frame_settings->m_frame_buffer[coll][row]), m_frame_item));
    //            /std::cout << (row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))<< std::endl;
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setScale(0.01);
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setPos(coll + 0.03, row + 0.035);
    auto pixel_value = m_frame_manager->get_pixel_value(row, coll);
    if (pixel_value >= pixel_threashold)
        m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setBrush(QBrush(QColor(Qt::black)));
    else
        m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setBrush(QBrush(QColor(Qt::white)));
}

bool Custom_view_frame::eventFilter(QObject *object, QEvent *event)
{
    static int zoom_value = 0;
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = m_target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            m_target_viewport_pos = mouse_event->pos();
            m_target_scene_pos = mapToScene(mouse_event->pos());
            //            std::cout << m_target_scene_pos.x() << std::endl;
            //            std::cout << m_target_scene_pos.y() << std::endl;
            //
            //mapToScene(m_frame_item->boundingRect().bottomRight().toPoint())
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        setDragMode(ScrollHandDrag);
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == Qt::NoModifier)
        {
            if (wheel_event->orientation() == Qt::Vertical)
            {
                double angle = wheel_event->angleDelta().y();
                if (angle > 0)
                    zoom_value++;
                else
                {
                    zoom_value--;
                }
                double factor = qPow(m_zoom_factor_base, angle);
                gentle_zoom(factor);
                return true;
            }
        }
        if (QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            static double contrast_value = 0.1;
            if (wheel_event->orientation() == Qt::Vertical)
            {
                double angle = wheel_event->angleDelta().y();
                if (angle > 0)
                    contrast_value = contrast_value + 0.5;
                else
                {
                    if (contrast_value > 0.5)
                        contrast_value = contrast_value - 0.5;
                }
                m_frame_manager->contrast_image(contrast_value);
                m_frame_item->setPixmap(m_frame_manager->add_image(m_rows, m_colls));
                return true;
            }
        }
    }


    if (zoom_value >= 26 && m_is_image_here)
    {
        Text_rect text_rect;
        text_rect.m_pos_x = (int)mapToScene(m_frame_item->boundingRect().topLeft().toPoint()).x();
        text_rect.m_pos_y = (int)mapToScene(m_frame_item->boundingRect().topLeft().toPoint()).y();
        text_rect.m_width = (int)mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).x();
        text_rect.m_height = (int)mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).y();
        uint16_t shift_coef_x = 0;
        uint16_t shift_coef_y = 0;
        if (default_resolution.height == m_rows && default_resolution.width == m_colls)
        {
            shift_coef_x = 0;
            shift_coef_y = 0;
        }
        else
        {
            shift_coef_x = default_resolution.width / m_colls;
            shift_coef_y = default_resolution.height / m_rows;
        }
        text_rect.m_width = text_rect.m_width + (text_rect.m_width - text_rect.m_pos_x) * shift_coef_x;
        text_rect.m_height = text_rect.m_height + (text_rect.m_height - text_rect.m_pos_y) * shift_coef_y;
        //std::cout << text_rect.m_width - text_rect.m_pos_x<< " " <<  text_rect.m_height - text_rect.m_pos_y << std::endl;
        //text_rect.m_width = text_rect.m_width + (text_rect.m_width - text_rect.m_pos_x);
        //        text_rect.m_height = text_rect.m_height + (text_rect.m_height - text_rect.m_pos_y);
        //text_rect.m_width = (int)mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).x() + (int)(mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).x() / 10);
        //text_rect.m_height = (int)mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).y() + (int)(mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).y() / 10);
        //        std::cout << m_frame_item->width() << " " << text_rect.m_height << std::endl;
        //        text_rect.m_pos_x = m_target_scene_pos.x() - 25;
        //        text_rect.m_pos_y = m_target_scene_pos.y() - 25;
        //        text_rect.m_width = m_target_scene_pos.x() + 25;
        //        text_rect.m_height = m_target_scene_pos.y() + 25;
        add_text_item_on_pixmap(text_rect);
    }
    else
        clear_text_item();
    Q_UNUSED(object)
    return false;
}


