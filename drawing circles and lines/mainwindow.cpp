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
#include <QMessageBox>

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
    else if (e->button() == Qt::RightButton) {
        if (firstClick) {
            firstPoint = e->pos();
            firstClick = false;
        } else {
            secondPoint = e->pos();
            firstClick = true;
            drawCircle(firstPoint, secondPoint);
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
    // instrukcja
    QMessageBox msgBox;
    msgBox.setWindowTitle("Instrukcja");
    msgBox.setText("Aby narysować linię, naciśnij lewy przycisk myszy.\n"
                   "Aby narysować koło, naciśnij prawy przycisk myszy.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.drawImage(0,0,image);

}


void MainWindow::drawCircle(QPoint p1, QPoint p2) {
    int x0 = p1.x();
    int y0 = p1.y();
    int dx = p2.x() - x0;
    int dy = p2.y() - y0;
    int r = int(std::sqrt(dx * dx + dy * dy));

    double d = 5.0-4*r;
    int x = 0;
    int y = r;
    double deltaA = (-2*r+5)*4;
    double deltaB = 3*4;
    auto setPixelInImage = [&](int x, int y) {
        if (x0 - x >= 0 && x0 - x < image.width() && y0 - y >= 0 && y0 - y < image.height())
            image.setPixel(x0 - x, y0 - y, qRgb(255, 255, 255));
        if (x0 - x >= 0 && x0 - x < image.width() && y0 + y >= 0 && y0 + y < image.height())
            image.setPixel(x0 - x, y0 + y, qRgb(255, 255, 255));
        if (x0 + x >= 0 && x0 + x < image.width() && y0 - y >= 0 && y0 - y < image.height())
            image.setPixel(x0 + x, y0 - y, qRgb(255, 255, 255));
        if (x0 + x >= 0 && x0 + x < image.width() && y0 + y >= 0 && y0 + y < image.height())
            image.setPixel(x0 + x, y0 + y, qRgb(255, 255, 255));
        if (x0 - y >= 0 && x0 - y < image.width() && y0 - x >= 0 && y0 - x < image.height())
            image.setPixel(x0 - y, y0 - x, qRgb(255, 255, 255));
        if (x0 - y >= 0 && x0 - y < image.width() && y0 + x >= 0 && y0 + x < image.height())
            image.setPixel(x0 - y, y0 + x, qRgb(255, 255, 255));
        if (x0 + y >= 0 && x0 + y < image.width() && y0 - x >= 0 && y0 - x < image.height())
            image.setPixel(x0 + y, y0 - x, qRgb(255, 255, 255));
        if (x0 + y >= 0 && x0 + y < image.width() && y0 + x >= 0 && y0 + x < image.height())
            image.setPixel(x0 + y, y0 + x, qRgb(255, 255, 255));
    };

    while (x <= y) {
        setPixelInImage(x, y);
        if (d > 0) {
            d += deltaA;
            y -= 1;
            x += 1;
            deltaA += 4*4;
            deltaB += 2*4;
        } else {
            d += deltaB;
            x += 1;
            deltaA += 2*4;
            deltaB += 2*4;
        }
    }
}
void MainWindow::drawLine(QPoint p1, QPoint p2)
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    int inc_dec = 1;

    if (abs(dy) > abs(dx)) {




        if (p2.y() < p1.y()) {
            std::swap(p1, p2);
            dx = -dx;
            dy = -dy;
        }

        if (p2.x() < p1.x()) {
            inc_dec = -1;
            dx = -dx;
        }


        // if (p2.x() > p1.x() && p2.y() < p1.y()) {
        //     inc_dec = -1;
        //     dy = -dy;
        // }

        // if(p2.x() < p1.x() && p2.y() > p1.y()) {
        //     std::swap(p1, p2);
        //     inc_dec = -1;
        //     // dx = -dx;
        //     // dy = -dy;
        // }

        int x = p1.x();
        int y = p1.y();
        int d = 2*dx - dy;
        int bi = 2*(dx - dy);
        int ai = 2*dx;

        for (; y <= p2.y(); y++) {
            if (d >= 0) {
                d += bi;
                x+= inc_dec;
            } else {
                d += ai;
            }
            image.setPixel(x, y, qRgb(255, 255, 255));
        }
    }
    else
    {
        if (p2.x() < p1.x()) {
            std::swap(p1, p2);
            dx = -dx;
            dy = -dy;
        }


        if (p2.y() < p1.y()) {
            inc_dec = -1;
            dy = -dy;
        }

        // if(p2.x() < p1.x() && p2.y() > p1.y()) {
        //     std::swap(p1, p2);
        //     inc_dec = -1;
        //     dx = -dx;
        //     dy = -dy;
        // }

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














