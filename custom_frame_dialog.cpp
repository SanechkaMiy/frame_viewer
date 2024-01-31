#include "custom_frame_dialog.h"
#include "ui_custom_frame_dialog.h"

Custom_frame_dialog::Custom_frame_dialog(uint16_t rows, uint16_t colls, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Custom_frame_dialog),
    m_rows(rows),
    m_colls(colls)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::Dialog);
    QGraphicsScene* frame_scene = new QGraphicsScene(this);
    m_frame_manager.reset(new Frame_manager(&m_frame_settings));
    ui->canvas_for_frame->setScene(frame_scene);
    m_frame_item = new QGraphicsPixmapItem();
    ui->canvas_for_frame->setBackgroundBrush(QBrush(Qt::black));
    m_frame_item->setFlags(QGraphicsItem::ItemIsMovable);
    frame_scene->addItem(m_frame_item);
    m_zoom_factor_base = 1.0015;
    ui->canvas_for_frame->viewport()->installEventFilter(this);
    ui->canvas_for_frame->setMouseTracking(true);
    QPen pen_rect_style;
    pen_rect_style.setWidthF(0.005);
    pen_rect_style.setColor(Qt::white);
    for (uint16_t num_item = 0; num_item  < m_pixel_threashold_to_show; num_item++)
    {
        m_text_item.push_back(new QGraphicsSimpleTextItem(m_frame_item));
        m_rect_item.push_back(new QGraphicsRectItem(m_frame_item));
        m_rect_item[num_item]->setPen(pen_rect_style);
        //        m_rect_item[num_item]->setBrush(Qt::NoBrush);

    }

}

void Custom_frame_dialog::set_frame_settings(uint16_t type_data, QString path)
{
    m_frame_manager->load_image(path, type_data, m_rows, m_colls);
    m_frame_item->setPixmap(m_frame_manager->add_image(m_rows, m_colls));
}

Custom_frame_dialog::~Custom_frame_dialog()
{
    for (auto el : m_text_item)
        delete el;
    m_text_item.clear();

    for (auto el : m_rect_item)
        delete el;
    m_rect_item.clear();

    delete ui;
}

void Custom_frame_dialog::gentle_zoom(double factor)
{
    ui->canvas_for_frame->scale(factor, factor);
    ui->canvas_for_frame->centerOn(m_target_scene_pos);
    QPointF delta_viewport_pos = m_target_viewport_pos - QPointF(ui->canvas_for_frame->viewport()->width() / 2.0,
                                                                 ui->canvas_for_frame->viewport()->height() / 2.0);
    QPointF viewport_center = ui->canvas_for_frame->mapFromScene(m_target_scene_pos) - delta_viewport_pos;
    ui->canvas_for_frame->centerOn(ui->canvas_for_frame->mapToScene(viewport_center.toPoint()));
}

void Custom_frame_dialog::clear_graphics_item()
{
    for (auto el : m_text_item)
        el->setVisible(false);

    for (auto el : m_rect_item)
        el->setVisible(false);
}

void Custom_frame_dialog::add_items_on_pixmap(Text_rect &text_rect)
{
    clear_graphics_item();
    for (uint16_t coll = text_rect.m_pos_x; coll < text_rect.m_width; ++coll)
    {
        for (uint16_t row = text_rect.m_pos_y; row < text_rect.m_height; ++row)
        {
            set_items(text_rect, coll, row);
        }
    }
}

void Custom_frame_dialog::set_items(Text_rect &text_rect, uint16_t &coll, uint16_t &row)
{
    m_rect_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setVisible(true);
    m_rect_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setRect(coll, row, 1, 1);

    //            /std::cout << (row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))<< std::endl;
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setVisible(true);
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setText(QString::number(m_frame_settings.m_frame_buffer[coll][row]));
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setScale(0.01);
    m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setPos(coll + 0.03, row + 0.035);
    auto pixel_value = m_frame_manager->get_pixel_value(row, coll);
    if (pixel_value >= pixel_threashold)
        m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setBrush(QBrush(QColor(Qt::black)));
    else
        m_text_item[(row - text_rect.m_pos_y) + ((coll - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y))]->setBrush(QBrush(QColor(Qt::white)));
}

void Custom_frame_dialog::set_positive_value(int16_t &value)
{
    if (value < 0)
        value = 0;
}

void Custom_frame_dialog::set_width_and_height_is_normalize(int16_t &width, int16_t &height)
{
    if (width > m_colls)
        width = m_colls;

    if (height > m_rows)
        height = m_rows;
}

bool Custom_frame_dialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = m_target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            m_target_viewport_pos = mouse_event->pos();
            m_target_scene_pos = ui->canvas_for_frame->mapToScene(mouse_event->pos());
            m_is_scroll = false;
            m_is_move = true;
            //

            //mapToScene(m_frame_item->boundingRect().bottomRight().toPoint())
        }
        //QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

    }
    else if (event->type() == QEvent::Wheel)
    {
        ui->canvas_for_frame->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == Qt::NoModifier)
        {
            if (wheel_event->orientation() == Qt::Vertical)
            {
                m_is_scroll = true;
                m_is_move = false;
                double angle = wheel_event->angleDelta().y();
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
    Text_rect text_rect;

    text_rect.m_pos_x = (int)ui->canvas_for_frame->mapToScene(m_frame_item->boundingRect().topLeft().toPoint()).x() - (int)m_frame_item->pos().x();
    text_rect.m_pos_y = (int)ui->canvas_for_frame->mapToScene(m_frame_item->boundingRect().topLeft().toPoint()).y() - (int)m_frame_item->pos().y();
    text_rect.m_width = (int)ui->canvas_for_frame->mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).x() - (int)m_frame_item->pos().x();
    text_rect.m_height = (int)ui->canvas_for_frame->mapToScene(m_frame_item->boundingRect().bottomRight().toPoint()).y() - (int)m_frame_item->pos().y();

    set_positive_value(text_rect.m_pos_x);
    set_positive_value(text_rect.m_pos_y);


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
    //    text_rect.m_width = text_rect.m_width + m_frame_item->pos().x();
    //    text_rect.m_height = text_rect.m_height + m_frame_item->pos().y();
    text_rect.m_width = text_rect.m_width + (text_rect.m_width - text_rect.m_pos_x) * shift_coef_x;
    text_rect.m_height = text_rect.m_height + (text_rect.m_height - text_rect.m_pos_y) * shift_coef_y;

    set_width_and_height_is_normalize(text_rect.m_width, text_rect.m_height);
    std::cout << text_rect.m_pos_x << " " << text_rect.m_pos_y<< " " << text_rect.m_width << " " << text_rect.m_height << std::endl;
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
    //add_text_item_on_pixmap(text_rect);

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton)
        {
            m_is_mouse_click = true;
        }
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton)
        {
            m_is_mouse_click = false;
        }
    }
    if (m_is_mouse_click & m_is_move || m_is_scroll)
    {
        if ((text_rect.m_width - text_rect.m_pos_x) * (text_rect.m_height - text_rect.m_pos_y) <= m_pixel_threashold_to_show)
        {
            add_items_on_pixmap(text_rect);
        }
        else
        {
            clear_graphics_item();
        }
    }
    Q_UNUSED(object)
    return false;
}
