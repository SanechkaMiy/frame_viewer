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
#include <QObject>
#include <QMimeData>
#include "custom_graphic_scene.h"
#include "custom_dialog_frame_settings.h"
#include <thread>
#include <QDragEnterEvent>
struct  Text_rect
{
    uint16_t m_pos_x;
    uint16_t m_pos_y;
    uint16_t m_width;
    uint16_t m_height;
};

struct  Frame_resolution
{
    Frame_resolution(uint16_t width, uint16_t height):
        width(width), height(height)
    {}
    uint16_t width;
    uint16_t height;
};
const uint16_t pixel_threashold = 100;
const Frame_resolution default_resolution(2048, 2048);
class Custom_view_frame : public QGraphicsView
{
public:
    explicit Custom_view_frame(QWidget *parent = nullptr);
private slots:
    void load_image_from_path(uint16_t rows, uint16_t colls);
private:
    QString m_path;
    void gentle_zoom(double factor);
    void clear_text_item();
    void add_text_item_on_pixmap(Text_rect& text_rect);
    void create_text_item(Text_rect& text_rect, uint16_t& coll, uint16_t& row);
    std::shared_ptr<Frame_settings> m_frame_settings;
    std::shared_ptr<Frame_manager> m_frame_manager;
    Custom_dialog_frame_settings* m_dialog_frame_settings;
    Custom_graphic_scene* m_graphic_scene;
    Frame_item* m_frame_item;
    double m_zoom_factor_base;
    std::vector<QGraphicsSimpleTextItem*> m_text_item;
    QPointF m_target_scene_pos, m_target_viewport_pos;
    bool m_is_image_here = false;
    uint16_t m_rows = 1024;
    uint16_t m_colls = 1024;
protected:
    //void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject* object, QEvent* event) override;

};

#endif // CUSTOM_VIEW_FRAME_H
