#include "Frame_manager.h"
#include <QDebug>


Frame_manager::Frame_manager(std::shared_ptr<Frame_settings> frame_settings) :
    m_frame_settings(frame_settings)
{
}

QPixmap Frame_manager::add_image()
{
    return m_pixmap;
}

void Frame_manager::mat_to_pixmap(cv::Mat img)
{
    m_pixmap = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8));
}

void Frame_manager::load_image()
{
    FILE* read_image = fopen("frame.mtx", "rb");
    if (read_image == NULL)
    {
        printf("Image Not Found\n");
    }

    fseek(read_image, 0, SEEK_END);
    int fileLen = ftell(read_image);
    fseek(read_image, 0, SEEK_SET);
    void* lpv_buf = ::operator new (fileLen);
    fread(lpv_buf, fileLen, 1, read_image);
    fclose(read_image);
    uint16_t* lpwLine;
    std::array<std::array<uchar, 2048>, 2048> arr;
    m_frame_settings->m_frame_buffer.resize(2048);
    for (uint16_t coll = 0; coll < 2048; coll++)
    {
        m_frame_settings->m_frame_buffer[coll].resize(2048);
        lpwLine = (uint16_t*)lpv_buf + coll * 2048;
        for (uint16_t row = 0; row < 2048; row++)
        {
            m_frame_settings->m_frame_buffer[coll][row] = lpwLine[row];
            arr[coll][row] = lpwLine[row] >> 2;
        }
    }
    cv::Mat img(2048, 2048, CV_8UC1, arr.data());
    m_img = img.clone();
    mat_to_pixmap(m_img);
}

void Frame_manager::contrast_image(double contrast_value)
{
    cv::Mat img;
    std::vector<cv::Mat> channels;
    cv::split(m_img, channels);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(contrast_value, cv::Size(1, 1));
    for(auto ch : channels)
        clahe->apply(ch, ch);
    cv::merge(channels, img);
    mat_to_pixmap(img);
}


