#include "label_image.h"


Label_image::Label_image(QWidget *parent)
    : QLabel(parent)
{
    setAttribute(Qt::WA_MouseTracking);
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setScaledContents(true);
    add_image();
}

void Label_image::add_image()
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
    for (uint16_t coll = 0; coll < 2048; coll++)
    {
        lpwLine = (uint16_t*)lpv_buf + coll * 2048;
        for (uint16_t row = 0; row < 2048; row++)
        {
            arr[coll][row] = lpwLine[row] >> 2;
        }
    }
    cv::Mat img(2048, 2048, CV_8UC1, arr.data());
    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(20, cv::Size(100, 100));
    for(auto ch : channels)
        clahe->apply(ch, ch);
    cv::merge(channels, img);
    setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8)));

  adjustSize();
}


