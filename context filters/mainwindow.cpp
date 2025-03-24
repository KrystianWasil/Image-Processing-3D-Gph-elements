#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered()),this,SLOT(open_image()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_image()
{
    if (!this->image.isNull()) {
        ui->label->setPixmap(QPixmap::fromImage(this->image_cpy));
    }

}

template<class T>
T clamp(T v, T minv, T maxv) {
    if(v<minv) return minv;
    if(v>maxv) return maxv;
    return v;
}

// r i amska 3 i -1 -1 -1
//               -1  9 -1
//               -1 -1 -1
void MainWindow::on_pushButton_clicked() {
    int r = 7;
    resize_image_with_border(r);
}

void MainWindow::resize_image_with_border(int r)
{
    int width = image.width();
    int height = image.height();
    int newWidth = width + 2 * r;
    int newHeight = height + 2 * r;
    mask =  {
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //9x9
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //9x9
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //9x9
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //9x9
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //9x9
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
        {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}

    };
    QImage resizedImage(newWidth, newHeight, image.format());

    //kopiowanie do srodka

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int srcX = clamp(x - r, 0, width - 1);
            int srcY = clamp(y - r, 0, height - 1);
            resizedImage.setPixel(x, y, image.pixel(srcX, srcY));
        }
    }




    // // kopia do srodka nowego onrazu
    // for (int y = 0; y < height; ++y) {
    //     for (int x = 0; x < width; ++x) {
    //         resizedImage.setPixel(x + r, y + r, image.pixel(x, y));
    //     }
    // }

    // //lewy gorny rog
    // for (int y = 0; y < r; ++ y) {
    //     for (int x = 0; x < r; ++ x) {
    //         resizedImage.setPixel(x, y, image.pixel(0, 0));
    //     }
    // }
    // // prawy gorny
    // for (int y = 0; y < r; ++ y) {
    //     for (int x = image.width() + r + 1; x < newWidth; ++ x) {
    //         resizedImage.setPixel(x, y, image.pixel(image.width() - 1, 0));
    //     }
    // }
    // //lewy dolny rog
    // for (int y = image.height() + r + 1; y < newHeight; ++ y) {
    //     for (int x = 0; x < r; ++ x) {
    //         resizedImage.setPixel(x, y, image.pixel(0, image.height() - 1));
    //     }
    // }

    // //prawy dolny rog
    // for (int y = image.height() + r + 1; y < newHeight; ++ y) {
    //     for (int x = image.width() + r + 1; x < newWidth; ++ x) {
    //         resizedImage.setPixel(x, y, image.pixel(image.width() -1 , image.height() -1 ));
    //     }
    // }

    //przykładanie maski
    const int maskSize = 2 * r + 1;
    const int maskSum = maskSize*maskSize;
    image_cpy = resizedImage;
    for (int y = r; y < height + r; ++y) {
        for (int x = r; x < width + r ; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            //maska
            for (int j = -r; j <= r; ++j) {
                for (int i = -r; i <= r; ++i) {
                    QColor color = QColor(resizedImage.pixel(x + i, y + j));
                    sumR += color.red() * mask[j + r][i + r];
                    sumG += color.green() * mask[j + r][i + r];
                    sumB += color.blue() * mask[j + r][i + r];
                }
            }
            int r = clamp(sumR/maskSum, 0, 255);
            int g = clamp(sumG/maskSum, 0, 255);
            int b = clamp(sumB/maskSum, 0, 255);

            image_cpy.setPixel(x, y, qRgb(r, g, b));

        }
    }


    show_image();
}


void MainWindow::open_image()
{

    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isNull()) {
        image.load(filename);
        show_image();
        image_cpy = image;
    }

    show_image();
}












