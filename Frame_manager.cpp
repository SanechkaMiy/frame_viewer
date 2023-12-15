#include "Frame_manager.h"
#include <QDebug>


Frame_manager::Frame_manager(std::shared_ptr<Frame_settings> frame_settings) :
    m_frame_settings(frame_settings)
{
}

QPixmap Frame_manager::add_image(const uint16_t& rows, const uint16_t& colls)
{
    if (m_img.empty())
    {
        std::cout << "empty" << std::endl;
        cv::Mat img(rows, colls, CV_8UC1);
        mat_to_pixmap(img);
    }
    return m_pixmap;
}

uint16_t Frame_manager::get_pixel_value(uint16_t &row, uint16_t &col)
{
    return (int)m_img.at<uchar>(row, col);
}

void Frame_manager::mat_to_pixmap(cv::Mat img)
{
    m_pixmap = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8));
}

void Frame_manager::load_image(const QString& path, const uint16_t& rows, const uint16_t& colls)
{
    FILE* read_image = fopen(path.toStdString().c_str(), "rb");
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
    std::array<std::array<uchar, 1024>, 1024> arr;
    //std::vector<std::vector<uchar>> arr(2048);
//    arr.resize(rows);
//    for (uint16_t row = 0; row < rows; row++)
//        arr[row].resize(colls);
    m_frame_settings->m_frame_buffer.resize(rows);
    for (uint16_t row = 0; row < rows; row++)
    {
        //arr[row].resize(colls);
        m_frame_settings->m_frame_buffer[row].resize(colls);
        lpwLine = (uint16_t*)lpv_buf + row * colls;
        for (uint16_t coll = 0; coll < colls; coll++)
        {
            m_frame_settings->m_frame_buffer[row][coll] = lpwLine[coll];
            arr[row][coll] = lpwLine[coll] / (4095 / 255); //>> 2;
        }
    }
    uint16_t temp;
    for (uint16_t row = 1; row < rows; row++)
    {
        for (uint16_t coll = 0; coll < row; coll++)
        {
            temp = m_frame_settings->m_frame_buffer[coll][row];
            m_frame_settings->m_frame_buffer[coll][row] = m_frame_settings->m_frame_buffer[row][coll];
            m_frame_settings->m_frame_buffer[row][coll] = temp;
        }
    }
    cv::Mat img(rows, colls, CV_8UC1, arr.data());
    m_img = img.clone();
    mat_to_pixmap(m_img);
}

//void Frame_manager::load_image(const QString& path, const uint16_t& rows, const uint16_t& colls)
//{
//    FILE* read_image = fopen(path.toStdString().c_str(), "rb");
//    if (read_image == NULL)
//    {
//        printf("Image Not Found\n");
//    }

//    fseek(read_image, 0, SEEK_END);
//    int fileLen = ftell(read_image);
//    fseek(read_image, 0, SEEK_SET);
//    void* lpv_buf = ::operator new (fileLen);
//    fread(lpv_buf, fileLen, 1, read_image);
//    fclose(read_image);
//    uint8_t* lpwLine;
//    std::array<std::array<uchar, 512>, 512> arr;
//    //std::vector<std::vector<uchar>> arr(2048);
//    //    arr.resize(rows);
//    //    for (uint16_t row = 0; row < rows; row++)
//    //        arr[row].resize(colls);
//    m_frame_settings->m_frame_buffer.resize(rows);
//    for (uint16_t row = 0; row < rows; row++)
//    {
//        //arr[row].resize(colls);
//        m_frame_settings->m_frame_buffer[row].resize(colls);
//        lpwLine = (uint8_t*)lpv_buf + row * colls;
//        for (uint16_t coll = 0; coll < colls; coll++)
//        {
//            m_frame_settings->m_frame_buffer[row][coll] = lpwLine[coll];
//            arr[row][coll] = lpwLine[coll]; //>> 2;
//        }
//    }
//    uint16_t temp;
//    for (uint16_t row = 1; row < rows; row++)
//    {
//        for (uint16_t coll = 0; coll < row; coll++)
//        {
//            temp = m_frame_settings->m_frame_buffer[coll][row];
//            m_frame_settings->m_frame_buffer[coll][row] = m_frame_settings->m_frame_buffer[row][coll];
//            m_frame_settings->m_frame_buffer[row][coll] = temp;
//        }
//    }
//    cv::Mat img(rows, colls, CV_8UC1, arr.data());
//    m_img = img.clone();

//    mat_to_pixmap(m_img);
//}

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


