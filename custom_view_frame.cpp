#include "custom_view_frame.h"
#include <QDebug>

Custom_view_frame::Custom_view_frame(QWidget *parent) :
    QGraphicsView(parent)
{
    m_frame_settings.reset(new Frame_settings());
    m_frame_manager.reset(new Frame_manager(m_frame_settings));
    m_graphic_scene = new Custom_graphic_scene(this);
    setScene(m_graphic_scene);
    m_empty_item = new Frame_item();
    m_graphic_scene->addItem(m_empty_item);
    //m_graphic_scene->removeItem(empty_item);
    //m_graphic_scene->addItem(m_frame_item);
    m_empty_item->setAcceptDrops(true);
    //m_frame_manager->load_image("path");
    width();
    height();
    m_empty_item->setPixmap(QPixmap(parent->width(), parent->height()));
    m_empty_item->setScale(100);
    connect(m_graphic_scene, &Custom_graphic_scene::send_url, this, [this](QString path)
    {
        m_dialog_frame_settings = new Custom_dialog_frame_settings(this);
        connect(m_dialog_frame_settings, &Custom_dialog_frame_settings::load_image, this, [this, path](uint16_t type_data, uint16_t rows, uint16_t colls)
        {
            //scale(15, 15);
            Custom_frame_dialog* frame = new Custom_frame_dialog(m_frame_settings,
                                                                 m_frame_manager,
                                                                 rows, colls, this);
            frame->setWindowTitle("Кадр " + QString::number(rows) + " на " + QString::number(colls));
            frame->set_frame_settings(type_data, path);
            //scale(0.1, 0.1);
            m_dialog_frame_settings->close();
            frame->show();
        });
        m_dialog_frame_settings->setModal(true);
        m_dialog_frame_settings->exec();

    });
}
