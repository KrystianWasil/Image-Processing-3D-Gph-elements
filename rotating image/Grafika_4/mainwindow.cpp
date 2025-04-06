#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered()),this,SLOT(open_image()));
    // connect(ui->Lslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
    // connect(ui->aslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
    // connect(ui->bslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(test()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_image()
{
    if (!this->image_cpy.isNull()) {
        ui->label->setFixedSize(image_cpy.size());

        ui->label->setPixmap(QPixmap::fromImage(this->image_cpy));
    }
}
template<class T>
T clamp(T v, T minv, T maxv) {
    if(v<minv) return minv;
    if(v>maxv) return maxv;
    return v;
}



void MainWindow::open_image()
{

    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isNull()) {
        image.load(filename);
        show_image();
        image_cpy = image;
    }
    // test();
    show_image();
}




// void MainWindow::test() {
//     skalowanie();
// }


void MainWindow::skalowanie() {
    if (image.isNull()) return;

    double sx = 10.0;
    double sy = 10.0;

    int newWidth = image.width() * sx;
    int newHeight = image.height() * sy;

    QImage scaledImage(newWidth, newHeight, image.format());


    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int origX = round(x / sx);
            int origY = round(y / sy);

            origX = clamp(origX, 0, image.width() - 1);
            origY = clamp(origY, 0, image.height() - 1);

            scaledImage.setPixelColor(x, y, image.pixelColor(origX, origY));
        }
    }
    image_cpy = scaledImage;
    show_image();
}


// void MainWindow::przesuniecieMacierzowe(int tx, int ty) {
//     if (image.isNull()) return;

//     QImage translatedImage(image.size(), image.format());
//     translatedImage.fill(Qt::transparent);

//     for (int y = 0; y < image.height(); ++y) {
//         for (int x = 0; x < image.width(); ++x) {
//             int newX = x - tx;
//             int newY = y - ty;

//             if (newX >= 0 && newX < image.width() &&
//                 newY >= 0 && newY < image.height()) {
//                 translatedImage.setPixelColor(x, y, image.pixelColor(newX, newY));
//             }
//         }
//     }

//     image_cpy = translatedImage;
//     show_image();
// }


// Dodaj tę linię na początku pliku dla funkcji trygonometrycznych

void MainWindow::obrocWzgledemSrodka(double angle) {
    if (image.isNull()) return;

    double centerX = image.width() / 2.0;
    double centerY = image.height() / 2.0;

    QImage rotatedImage(image.size(), image.format());
    rotatedImage.fill(Qt::white);  // Wypełniamy tło obrazem białym

    double radians = angle * M_PI / 180.0;
    double cosA = cos(radians);
    double sinA = sin(radians);

    for (int y = 0; y < rotatedImage.height(); ++y) {
        for (int x = 0; x < rotatedImage.width(); ++x) {
            // Przesunięcie współrzędnych względem środka
            double xRel = x - centerX;
            double yRel = y - centerY;

            // Obliczenie współrzędnych w oryginalnym obrazie
            double origX = centerX + (xRel * cosA + yRel * sinA);
            double origY = centerY + (-xRel * sinA + yRel * cosA);



            // Sprawdzenie granic obrazu
            if (origX >= 0 && origX < image.width() &&
                origY >= 0 && origY < image.height()) {
                rotatedImage.setPixelColor(x, y, image.pixelColor(origX, origY));
            }
        }
    }

    image_cpy = rotatedImage;
    show_image();
}

// Możesz wywołać tę funkcję np. w metodzie test():
void MainWindow::test() {
    obrocWzgledemSrodka(45.0); // Obrót o 45 stopni
}

void MainWindow::przesuniecieDoSrodka() {
    obrocWzgledemSrodka(25);
}




















