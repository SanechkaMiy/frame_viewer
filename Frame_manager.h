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
#include <cstdio>
class Frame_manager
{
public:
    Frame_manager(std::shared_ptr<Frame_settings> frame_settings);
    void load_image();
    void contrast_image(double contrast_value);
    QPixmap add_image();
private:
    std::shared_ptr<Frame_settings> m_frame_settings;
    void mat_to_pixmap(cv::Mat img);
    QPixmap m_pixmap;
    cv::Mat m_img;
};

#endif // FRAME_MANAGER_H
