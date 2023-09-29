#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_label_image = new Label_image(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers () == Qt::CTRL)
    {
        m_image_left = m_label_image->pos();
        m_cursor_pos = (QCursor::pos() - pos()) - QPoint(1, 26);
        std::cout << m_image_left.x() << " " << m_cursor_pos.x() << " " << m_image_left.y() << " " << m_cursor_pos.y() <<  std::endl;
        if (event->angleDelta().y() > 0)
        {
            zoom_in();
        }
        else
        {
            zoom_out();
        }
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    auto pos = (m_dragStartPosition - event->pos());
    m_label_image->move(m_start_image_position - pos);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_start_image_position = m_label_image->pos();
        m_dragStartPosition = event->pos();
    }

}


void MainWindow::zoom_in()
{
    scale_image(1.5);
    w = w + 50;
    h = h + 50;
}

void MainWindow::zoom_out()
{
    scale_image(0.5);
    h = h - 50;
    w = w - 50;
}
void MainWindow::scale_image(double factor)
{
    m_scale_factor = factor;
    //std::cout << m_scale_factor;
    m_label_image->resize(m_scale_factor * m_label_image->pixmap()->size().width(), m_scale_factor * m_label_image->pixmap()->size().height());
    if (factor > 1)
        m_label_image->move((m_image_left.x() - m_cursor_pos.x()) * m_scale_factor + m_image_left.x(), (m_image_left.y() - m_cursor_pos.y()) * m_scale_factor + m_image_left.y());
    else
         m_label_image->move((m_image_left.x() - m_cursor_pos.x()) * m_scale_factor - m_image_left.x(), (m_image_left.y() - m_cursor_pos.y()) * m_scale_factor - m_image_left.y());
}
