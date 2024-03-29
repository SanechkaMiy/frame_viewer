#include "custom_view_frame.h"
#include <QDebug>

Custom_view_frame::Custom_view_frame(QWidget *parent) :
    QGraphicsView(parent)
{
    m_dialog_frame_settings = new Custom_dialog_frame_settings(this);
    m_dialog_frame_settings->setModal(true);
    connect(m_dialog_frame_settings, &Custom_dialog_frame_settings::load_image, this, [this](uint16_t type_data, uint16_t rows, uint16_t colls)
    {
        //scale(15, 15);
        auto frame = new Custom_frame_dialog(rows, colls, this);
        frame->setWindowTitle("Кадр " + QString::number(rows) + " на " + QString::number(colls));
        frame->set_frame_settings(type_data, m_path);
        //scale(0.1, 0.1);
        m_dialog_frame_settings->close();
        frame->show();
        m_count_frame++;

    });
    m_graphic_scene = new Custom_graphic_scene(this);
    setScene(m_graphic_scene);
    m_empty_item = new Frame_item();
    m_graphic_scene->addItem(m_empty_item);
    m_empty_item->setAcceptDrops(true);
    auto pixmap = QPixmap(parent->width(), parent->height());
    pixmap.fill(Qt::black);
    m_empty_item->setPixmap(pixmap);
    m_empty_item->setScale(100);
    connect(m_graphic_scene, &Custom_graphic_scene::send_url, this, [this](QString path)
    {
        m_path = path;
        m_dialog_frame_settings->exec();

    });
}
