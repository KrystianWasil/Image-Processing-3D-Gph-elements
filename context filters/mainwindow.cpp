#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <vector>
#include <algorithm>
#include <complex>
#include <cmath>
#include <fftw3.h>

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
    // int r = 7;
    // int r = 3;
    // int r = 1;
    int r = 10;
    resize_image_with_border(r);
}

void MainWindow::resize_image_with_border(int r)
{
    int width = image.width();
    int height = image.height();
    int newWidth = width + 2 * r;
    int newHeight = height + 2 * r;
    // mask =  {
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //15x15
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}

    // };
    //21x21
    // std::vector<std::vector<int>> mask;
    // for (int i = 0; i < 21; i++) {
    //     for (int j = 0; j < 21; j++) {
    //         mask[i][j] = 1;
    //     }

    // }

    int mask[21][21] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };



    // mask =  {
    //     {1,1, 1, 1, 1, 1, 1,1,1}, //9x9
    //     {1,1, 1, 1, 1, 1, 1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1}, //9x9
    //     {1,1, 1, 1, 1, 1, 1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1}, //9x9
    //     {1,1, 1, 1, 1, 1, 1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1}
    // };

    // mask =  {
    //     {1,1, 1}, //3x3
    //     {1,1, 1},
    //     {1,1, 1}
    // };

    // mask =  {
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}, //15x15
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1},
    //     {1,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1}

    // };

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
    QImage croppedImage(width, height, image_cpy.format());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            croppedImage.setPixel(x, y, image_cpy.pixel(x + r, y + r));
        }
    }

    image_cpy = croppedImage;


    show_image();
}

// void MainWindow::fft1d(std::vector<std::complex<double>>& data, bool invert) {
//     int n = data.size();
//     if (n <= 1) return;

//     std::vector<int> rev(n);
//     int log_n = 0;
//     while ((1 << log_n) < n) log_n++;

//     for (int i = 0, j = 0; i < n; i++) {
//         if (i < j) std::swap(data[i], data[j]);
//         int bit = n >> 1;
//         while (j & bit) {
//             j ^= bit;
//             bit >>= 1;
//         }
//         j ^= bit;
//     }

//     std::vector<std::complex<double>> wlen(n / 2);
//     for (int len = 2; len <= n; len <<= 1) {
//         double angle = 2 * M_PI / len * (invert ? -1 : 1);
//         std::complex<double> wn(cos(angle), sin(angle));
//         wlen[0] = 1;
//         for (int i = 1; i < len / 2; i++) {
//             wlen[i] = wlen[i - 1] * wn;
//         }
//         for (int i = 0; i < n; i += len) {
//             for (int j = 0; j < len / 2; j++) {
//                 std::complex<double> u = data[i + j];
//                 std::complex<double> v = data[i + j + len / 2] * wlen[j];
//                 data[i + j] = u + v;
//                 data[i + j + len / 2] = u - v;
//             }
//         }
//     }

//     if (invert) {
//         for (auto& x : data) x /= n;
//     }
// }

// void MainWindow::fft2d(std::vector<std::vector<std::complex<double>>>& data, bool invert) {
//     int height = data.size();
//     int width = data[0].size();

//     for (int y = 0; y < height; y++) {
//         fft1d(data[y], invert);
//     }

//     for (int x = 0; x < width; x++) {
//         std::vector<std::complex<double>> column(height);
//         for (int y = 0; y < height; y++) {
//             column[y] = data[y][x];
//         }
//         fft1d(column, invert);
//         for (int y = 0; y < height; y++) {
//             data[y][x] = column[y];
//         }
//     }
// }

// void MainWindow::test_fft() {
//     int width = image.width();
//     int height = image.height();

//     std::vector<std::vector<std::complex<double>>> imageDataR(height, std::vector<std::complex<double>>(width));
//     std::vector<std::vector<std::complex<double>>> imageDataG(height, std::vector<std::complex<double>>(width));
//     std::vector<std::vector<std::complex<double>>> imageDataB(height, std::vector<std::complex<double>>(width));

//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             QColor color = QColor(image.pixel(x, y));
//             imageDataR[y][x] = std::complex<double>(color.red(), 0);
//             imageDataG[y][x] = std::complex<double>(color.green(), 0);
//             imageDataB[y][x] = std::complex<double>(color.blue(), 0);
//         }
//     }

//     fft2d(imageDataR, false);
//     fft2d(imageDataG, false);
//     fft2d(imageDataB, false);

//
//     std::vector<std::vector<std::complex<double>>> blurMask(height, std::vector<std::complex<double>>(width, {0, 0}));
//     for (int y = 0; y < 15; y++) {
//         for (int x = 0; x < 15; x++) {
//             blurMask[y][x] = {1.0 / 225.0, 0};
//         }
//     }

//     fft2d(blurMask, false);

//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             imageDataR[y][x] *= blurMask[y][x];
//             imageDataG[y][x] *= blurMask[y][x];
//             imageDataB[y][x] *= blurMask[y][x];
//         }
//     }

//     fft2d(imageDataR, true);
//     fft2d(imageDataG, true);
//     fft2d(imageDataB, true);

//     QImage transformedImage(width, height, image.format());
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             int r = std::clamp(static_cast<int>(std::abs(imageDataR[y][x])), 0, 255);
//             int g = std::clamp(static_cast<int>(std::abs(imageDataG[y][x])), 0, 255);
//             int b = std::clamp(static_cast<int>(std::abs(imageDataB[y][x])), 0, 255);
//             transformedImage.setPixel(x, y, qRgb(r, g, b));
//         }
//     }

//     image_cpy = transformedImage;
//     show_image();
// }


//

// void MainWindow::fft2dUsingFFTW() {
//     int width = image.width();
//     int height = image.height();
//     int kernel_size = 15;

//
//     int padded_width = width + kernel_size - 1;
//     int padded_height = height + kernel_size - 1;

//
//     fftw_complex* dataR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);
//     fftw_complex* dataG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);
//     fftw_complex* dataB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);

//
//     fftw_complex* kernel = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);

//
//     for (int i = 0; i < padded_width*padded_height; i++) {
//         dataR[i][0] = dataR[i][1] = 0.0;
//         dataG[i][0] = dataG[i][1] = 0.0;
//         dataB[i][0] = dataB[i][1] = 0.0;
//         kernel[i][0] = kernel[i][1] = 0.0;
//     }

//
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             QColor color = QColor(image.pixel(x, y));
//             dataR[y * padded_width + x][0] = color.red();
//             dataG[y * padded_width + x][0] = color.green();
//             dataB[y * padded_width + x][0] = color.blue();
//         }
//     }

//
//     double kernel_sum = 0.0;
//     for (int y = 0; y < kernel_size; y++) {
//         for (int x = 0; x < kernel_size; x++) {
//             kernel[y * padded_width + x][0] = 1.0;
//             kernel_sum += 1.0;
//         }
//     }
//
//     for (int i = 0; i < padded_width*padded_height; i++) {
//         kernel[i][0] /= kernel_sum;
//     }

//
//     fftw_plan planR = fftw_plan_dft_2d(padded_height, padded_width, dataR, dataR, FFTW_FORWARD, FFTW_ESTIMATE);
//     fftw_plan planG = fftw_plan_dft_2d(padded_height, padded_width, dataG, dataG, FFTW_FORWARD, FFTW_ESTIMATE);
//     fftw_plan planB = fftw_plan_dft_2d(padded_height, padded_width, dataB, dataB, FFTW_FORWARD, FFTW_ESTIMATE);
//     fftw_plan planKernel = fftw_plan_dft_2d(padded_height, padded_width, kernel, kernel, FFTW_FORWARD, FFTW_ESTIMATE);

//
//     fftw_execute(planR);
//     fftw_execute(planG);
//     fftw_execute(planB);
//     fftw_execute(planKernel);

//
//     for (int i = 0; i < padded_width*padded_height; i++) {
//         // Complex multiplication: (a+ib)*(c+id) = (ac-bd) + i(ad+bc)
//         double reR = dataR[i][0] * kernel[i][0] - dataR[i][1] * kernel[i][1];
//         double imR = dataR[i][0] * kernel[i][1] + dataR[i][1] * kernel[i][0];
//         dataR[i][0] = reR;
//         dataR[i][1] = imR;

//         double reG = dataG[i][0] * kernel[i][0] - dataG[i][1] * kernel[i][1];
//         double imG = dataG[i][0] * kernel[i][1] + dataG[i][1] * kernel[i][0];
//         dataG[i][0] = reG;
//         dataG[i][1] = imG;

//         double reB = dataB[i][0] * kernel[i][0] - dataB[i][1] * kernel[i][1];
//         double imB = dataB[i][0] * kernel[i][1] + dataB[i][1] * kernel[i][0];
//         dataB[i][0] = reB;
//         dataB[i][1] = imB;
//     }

//
//     fftw_plan inv_planR = fftw_plan_dft_2d(padded_height, padded_width, dataR, dataR, FFTW_BACKWARD, FFTW_ESTIMATE);
//     fftw_plan inv_planG = fftw_plan_dft_2d(padded_height, padded_width, dataG, dataG, FFTW_BACKWARD, FFTW_ESTIMATE);
//     fftw_plan inv_planB = fftw_plan_dft_2d(padded_height, padded_width, dataB, dataB, FFTW_BACKWARD, FFTW_ESTIMATE);

//
//     fftw_execute(inv_planR);
//     fftw_execute(inv_planG);
//     fftw_execute(inv_planB);

//
//     QImage transformedImage(width, height, image.format());

//
//     double norm = padded_width * padded_height;
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             int r = std::clamp(static_cast<int>(dataR[y * padded_width + x][0] / norm), 0, 255);
//             int g = std::clamp(static_cast<int>(dataG[y * padded_width + x][0] / norm), 0, 255);
//             int b = std::clamp(static_cast<int>(dataB[y * padded_width + x][0] / norm), 0, 255);
//             transformedImage.setPixel(x, y, qRgb(r, g, b));
//         }
//     }

//
//     fftw_destroy_plan(planR);
//     fftw_destroy_plan(planG);
//     fftw_destroy_plan(planB);
//     fftw_destroy_plan(planKernel);
//     fftw_destroy_plan(inv_planR);
//     fftw_destroy_plan(inv_planG);
//     fftw_destroy_plan(inv_planB);
//     fftw_free(dataR);
//     fftw_free(dataG);
//     fftw_free(dataB);
//     fftw_free(kernel);

//
//     image_cpy = transformedImage;
//     show_image();
// }






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
