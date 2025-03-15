#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include "HistogramWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // chartViewR = new QChartView();
    // ui->gridLayout_2->addWidget(chartViewR, 0, 0);
    connect(ui->actionopen, SIGNAL(triggered()),this,SLOT(open_image()));
    connect(ui->verticalSlider, SIGNAL(valueChanged(int)),this,SLOT(brightenss(int)));
    connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)),this,SLOT(contrast(int)));
    connect(ui->verticalSlider_3, SIGNAL(valueChanged(int)),this,SLOT(gamma(int)));
    histogramWidget = ui->widget;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_image()
{
    if (!this->image.isNull()) {
        ui->label->setPixmap(QPixmap::fromImage(this->image_cpy));
        histogramWidget->setImage(image_cpy);
    }


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


template<class T>
T clamp(T v, T minv, T maxv) {
    if(v<minv) return minv;
    if(v>maxv) return maxv;
    return v;
}

void MainWindow::brightenss(int v) {
    brightness_value = v;
    createLUT(brightness_value, contrast_value, gamma_value);
    processImage();
}

// uchar MainWindow::processContrast(uchar v, float c) {
//     if (c >= 0) {
//         return (127.0f / (127.0f - c)) * (v - c),0.0f,255.0f;
//     }
//     else if (c < 0) {
//         return ((127.0f + c)/127.0f) * v - c ,0.0f,255.0f;
//     }
// }

void MainWindow::processImage() {
    // set0 = new QBarSet("Wartości r");
    uchar* bits = image.bits();
    uchar* process_bits = image_cpy.bits();

    // std::fill(histR, histR + 256, 0);
    // std::fill(histG, histG + 256, 0);
    // std::fill(histB, histB + 256, 0);



    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            int index = 4 * (y * image.width() + x);

            process_bits[index] = look_up_table[bits[index]]; // b
            process_bits[index + 1] = look_up_table[bits[index + 1]]; //g
            process_bits[index + 2] = look_up_table[bits[index + 2]];//r

        }
    }
    //normalize();
    // set0->remove(0,set0->count());
    // for (int i=0;i<256;++i){
    //     *set0 << histR[i];
    // }

    show_image();

}

// void MainWindow::normalize() {
//     int totalPixels = image.width() * image.height();

//     // for (int i = 0; i < 256;++i) {
//     //     // histB[i] /= totalPixels;
//     //     // histG[i] /= totalPixels;
//     //     histR[i] = histR[i]/ totalPixels;

//     //}

//     //sprawdzam czy sie noramlzuje powinno dawac1
//     //float sum = 0;

//     // for (int i = 0 ; i < 256; ++i) {
//     //     sum += histR[i];
//     // }
//     // qDebug() << sum;

// }



void MainWindow::createLUT(int brightness, int contrast, float gamma) {
    for (int i = 0; i < 256; ++i) {
        // jasnosc
        float value = i + brightness;

        // kontrast

        if (contrast == 127.0f) {
            if (value - contrast > 0) value = 255.0f;
            else value = 0;
        }
        else if (contrast >= 0) {
            value = (127.0f / (127.0f - contrast)) * (value - contrast);
        } else  {
            value = ((127.0f + contrast) / 127.0f) * value - contrast;
        }


        // gamma
        float normalized = clamp(value, 0.0f, 255.0f) / 255.0f;
        float transformed = std::pow(normalized, gamma);
        value = transformed * 255;

        // przredzial
        look_up_table[i] = clamp(value, 0.0f, 255.0f);

    }
}



void MainWindow::contrast(int c) {
    contrast_value = c;
    createLUT(brightness_value, contrast_value, gamma_value);
    processImage();
}

// void MainWindow::createGammaTable(float gamma, float* gamma_table) {
//     for (int i = 0; i <= 255; ++i) {
//         float normalized = i / 255.0f;
//         float transformed = std::pow(normalized, gamma);
//         gamma_table[i] = std::min(transformed * 255, 255.0f);
//     }
// }


void MainWindow::gamma(int g) {
    gamma_value = (g + 100) / 100.0f;
    createLUT(brightness_value, contrast_value, gamma_value);
    processImage();
}







