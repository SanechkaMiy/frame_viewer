#include "Frame_manager.h"
#include <QDebug>


Frame_manager::Frame_manager(Frame_settings *frame_settings) :
    m_frame_settings(frame_settings)
{
    create_frame_proc();
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

void Frame_manager::create_frame_proc()
{
    auto func_u16_frame_proc = [this](void* lpv_buf, uint16_t rows, uint16_t colls)
    {

        uint16_t* lpwLine;
        std::array<std::array<uchar, 2048>, 2048> arr;
        std::vector<uchar> arr_buf;
        arr_buf.resize(rows * colls);
        m_frame_settings->m_frame_buffer.resize(rows);
        for (uint16_t row = 0; row < rows; row++)
        {
            m_frame_settings->m_frame_buffer[row].resize(colls);
            lpwLine = (uint16_t*)lpv_buf + row * colls;
            for (uint16_t coll = 0; coll < colls; coll++)
            {
                m_frame_settings->m_frame_buffer[row][coll] = lpwLine[coll];
                arr[row][coll] = lpwLine[coll] / (4095 / 255); //>> 2;
                arr_buf[coll + row * rows] = arr[row][coll];
            }
        }
        return arr_buf;
    };
    m_func_frame_proc.push_back(func_u16_frame_proc);

    auto func_u8_frame_proc = [this](void* lpv_buf, uint16_t rows, uint16_t colls)
    {

        uint8_t* lpwLine;
        std::array<std::array<uchar, 2048>, 2048> arr;
        std::vector<uchar> arr_buf;
        arr_buf.resize(rows * colls);
        m_frame_settings->m_frame_buffer.resize(rows);
        for (uint16_t row = 0; row < rows; row++)
        {
            m_frame_settings->m_frame_buffer[row].resize(colls);
            lpwLine = (uint8_t*)lpv_buf + row * colls;
            for (uint16_t coll = 0; coll < colls; coll++)
            {
                m_frame_settings->m_frame_buffer[row][coll] = lpwLine[coll];
                arr[row][coll] = lpwLine[coll];
                arr_buf[coll + row * rows] = arr[row][coll];
            }
        }
        return arr_buf;
    };
    m_func_frame_proc.push_back(func_u8_frame_proc);

}


void Frame_manager::mat_to_pixmap(cv::Mat img)
{
    m_pixmap = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8));
}

void Frame_manager::load_image(const QString& path,
                               uint16_t type_data,
                               const uint16_t& rows, const uint16_t& colls)
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
    std::cout << type_data << std::endl;
    auto arr_buf = m_func_frame_proc[type_data](lpv_buf, rows, colls);

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

    cv::Mat img(rows, colls, CV_8UC1, arr_buf.data());
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


