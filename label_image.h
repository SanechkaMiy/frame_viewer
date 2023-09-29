#ifndef LABEL_IMAGE_H
#define LABEL_IMAGE_H

#include <QLabel>
#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
class Label_image : public QLabel
{
    Q_OBJECT
public:
    Label_image(QWidget *parent = nullptr);
private:
    void add_image();
};

#endif // LABEL_IMAGE_H
