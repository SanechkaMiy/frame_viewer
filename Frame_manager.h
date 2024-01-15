#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H
#include <QPixmap>
#include "frame_data.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <string.h>
#include <cstdio>
class Frame_manager
{
public:
    Frame_manager(std::shared_ptr<Frame_settings> frame_settings);
    void load_image(const QString &path,
                    uint16_t type_data,
                    const uint16_t& rows, const uint16_t& colls);
    void contrast_image(double contrast_value);
    QPixmap add_image(const uint16_t& rows, const uint16_t& colls);
    uint16_t get_pixel_value(uint16_t& row, uint16_t& col);
private:
    void create_frame_proc();
    std::shared_ptr<Frame_settings> m_frame_settings;
    void mat_to_pixmap(cv::Mat img);
    QPixmap m_pixmap;
    cv::Mat m_img;
    std::vector<std::function<std::vector<uchar>(void* lpv_buf, uint16_t rows, uint16_t colls)>> m_func_frame_proc;
};

#endif // FRAME_MANAGER_H
