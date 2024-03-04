#ifndef CUSTOM_FRAME_DIALOG_H
#define CUSTOM_FRAME_DIALOG_H
#include <QApplication>
#include <QDialog>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <qmath.h>
#include "frame_item.h"
#include "frame_data.h"
#include "Frame_manager.h"
#include <QObject>
#include <QMimeData>
#include <thread>
namespace Ui {
class Custom_frame_dialog;
}

struct  Text_rect
{
    int16_t m_pos_x;
    int16_t m_pos_y;
    int16_t m_width;
    int16_t m_height;
};

const uint16_t pixel_threashold = 100;
const int32_t m_pixel_threashold_to_show = 25 * 25;
class Custom_frame_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_frame_dialog(uint16_t rows, uint16_t colls, QWidget *parent = nullptr);
    void set_frame_settings(uint16_t type_data, QString path);
    ~Custom_frame_dialog();
private:
    Ui::Custom_frame_dialog *ui;
    std::shared_ptr<Frame_manager> m_frame_manager;
    Frame_settings m_frame_settings;
    QGraphicsPixmapItem* m_frame_item;
    double m_zoom_factor_base;
    std::vector<QGraphicsSimpleTextItem*> m_text_item;
    std::vector<QGraphicsRectItem*> m_rect_item;
    QPointF m_target_scene_pos, m_target_viewport_pos;
    uint16_t m_rows;
    uint16_t m_colls ;
    bool m_is_scroll = false;
    bool m_is_move = false;
    bool m_is_mouse_click = false;
    void gentle_zoom(double factor);
    void clear_graphics_item();
    void add_items_on_pixmap(Text_rect& text_rect);
    void set_items(Text_rect& text_rect, uint16_t& coll, uint16_t& row);
    void set_positive_value(int16_t& value);
    void set_width_and_height_is_normalize(int16_t& width, int16_t& height);
protected:
    bool eventFilter(QObject* object, QEvent* event) override;

};

#endif // CUSTOM_FRAME_DIALOG_H
