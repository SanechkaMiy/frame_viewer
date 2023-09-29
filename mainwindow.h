#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <math.h>
#include <label_image.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Label_image* m_label_image;
    QPoint m_dragStartPosition;
    QPoint m_start_image_position;
    QPoint m_image_left;
    QPoint m_cursor_pos;
    void scale_image(double factor);
    void zoom_in();
    void zoom_out();
    double m_scale_factor = 1.0;
    int w = 0, h = 0;

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

};
#endif // MAINWINDOW_H
