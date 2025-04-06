#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

}

MainWindow::~MainWindow()
{
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    // QPoint p = e->pos();
    // p.x();
    // p.y();
    // qDebug() << e->pos().x();
    // if (e->button() == Qt:q:LeftButton) {}
    if (e->button() == Qt::LeftButton) {
        if (firstClick) {
            firstPoint = e->pos();
            firstClick = false;
        } else {
            secondPoint = e->pos();
            firstClick = true;
            drawLine(firstPoint, secondPoint);
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == Qt::ShiftModifier) {}
    if (e->key() == Qt::Key_P) {}
    qDebug() << e->key();
    update();
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    image = QImage(e->size(),QImage::Format_RGB32);
    image.fill(qRgb(0,0,20));
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.drawImage(0,0,image);

}


void MainWindow::drawLine(QPoint p1, QPoint p2)
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();

    if (abs(dy) > abs(dx)) {
        // Obsługa linii pionowych (pominięte dla skupienia się na problemie)
    }
    else
    {
        int inc_dec = 1; // Domyślnie y rośnie

        // Zamiana punktów jeśli x2 < x1 (zawsze rysujemy od lewej do prawej)
        if (p2.x() < p1.x()) {
            std::swap(p1, p2);
            dx = -dx;
            dy = -dy;
        }

        // Ustalamy kierunek y
        if (p2.x() > p1.x() && p2.y() < p1.y()) {
            inc_dec = -1;
            dy = -dy;
        }

        if(p2.x() < p1.x() && p2.y() > p1.y()) {
            std::swap(p1, p2);
            inc_dec = -1;
            dx = -dx;
            dy = -dy;
        }

        int x = p1.x();
        int y = p1.y();
        int d = 2*dy - dx;
        int bi = 2*(dy - dx);
        int ai = 2*dy;

        for (; x <= p2.x(); x++) {
            if (d >= 0) {
                d += bi;
                y += inc_dec;
            } else {
                d += ai;
            }
            image.setPixel(x, y, qRgb(255, 255, 255));
        }
    }
}





// void MainWindow::show_image()
// {
//     if (!this->image.isNull()) {
//         ui->label->setPixmap(QPixmap::fromImage(this->image_cpy));
//     }
// }



// void MainWindow::open_image()
// {

//     QString filename = QFileDialog::getOpenFileName();
//     if (!filename.isNull()) {
//         image.load(filename);
//         show_image();
//         image_cpy = image;
//     }
//     test();
//     show_image();
// }


// template<class T>
// T clamp(T v, T minv, T maxv) {
//     if(v<minv) return minv;
//     if(v>maxv) return maxv;
//     return v;
// }














// {
//     int dx = p2.x() - p1.x();
//     int dy = p2.y() - p1.y();
//     if (abs(dy) > abs(dx)) {
//         //pionowo po y jak masz zdjecie to idziesz opo tych trojaktach gornym i dolnym
//     } else {
//         int x = p1.x();
//         int y = p1.y();
//         int dx = p2.x() - p1.x();
//         int dy = p2.y() - p1.y();

//         int d = 2*dy - dx;
//         int bi = 2*(dy - dx);
//         int ai = 2*dy;
//         if (p2.x() > p1.x() && p2.y() > p1.y()) {
//             for (;x!=p2.x();x++) {
//                 if (d >= 0) {
//                     d += bi;
//                     ++y;
//                 }
//                 else {
//                     d += ai;
//                 }
//                 image.setPixel(x,y,qRgb(255,255,255));
//             }
//         }
//         else if (p2.x() < p1.x() && p2.y() < p1.y()) {
//             std::swap(p1,p2);
//             x = p1.x();
//             y = p1.y();
//             int dx = p2.x() - p1.x();
//             int dy = p2.y() - p1.y();
//             int d = 2*dy - dx;
//             int bi = 2*(dy - dx);
//             int ai = 2*dy;

//             for (;x!=p2.x();x++) {
//                 if (d >= 0) {
//                     d += bi;
//                     ++y;
//                 }
//                 else {
//                     d += ai;
//                 }
//                 image.setPixel(x,y,qRgb(255,255,255));
//             }
//         }
//     }














