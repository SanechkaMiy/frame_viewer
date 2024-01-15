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
const int32_t m_pixel_threashold_to_show = 20 * 19;
class Custom_frame_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_frame_dialog(std::shared_ptr<Frame_settings> frame_settings,
                                 std::shared_ptr<Frame_manager> frame_manager,
                                 uint16_t rows, uint16_t colls, QWidget *parent = nullptr);
    void set_frame_settings(uint16_t type_data, QString path);
    ~Custom_frame_dialog();

private:
    Ui::Custom_frame_dialog *ui;
    std::shared_ptr<Frame_settings> m_frame_settings;
    std::shared_ptr<Frame_manager> m_frame_manager;
    Frame_item* m_frame_item;
    double m_zoom_factor_base;
    std::vector<QGraphicsSimpleTextItem*> m_text_item;
    QPointF m_target_scene_pos, m_target_viewport_pos;
    uint16_t m_rows;
    uint16_t m_colls ;

    void gentle_zoom(double factor);
    void clear_text_item();
    void add_text_item_on_pixmap(Text_rect& text_rect);
    void create_text_item(Text_rect& text_rect, uint16_t& coll, uint16_t& row);
protected:
    bool eventFilter(QObject* object, QEvent* event) override;

};

#endif // CUSTOM_FRAME_DIALOG_H
