#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered()),this,SLOT(open_image()));
    connect(ui->Lslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
    connect(ui->aslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
    connect(ui->bslider, SIGNAL(valueChanged(int)),this,SLOT(test()));
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



void MainWindow::open_image()
{

    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isNull()) {
        image.load(filename);
        show_image();
        image_cpy = image;
    }
    test();
    show_image();
}


template<class T>
T clamp(T v, T minv, T maxv) {
    if(v<minv) return minv;
    if(v>maxv) return maxv;
    return v;
}

void MainWindow::rgb2xyz(double r, double g, double b, double& Xx, double& Yy, double&Zz) {


    Xx = r * 0.57667 + g * 0.18556 + b * 0.18823;
    Yy = r * 0.29734 + g * 0.62736 + b * 0.07529;
    Zz = r * 0.02703 + g *0.07069 + b * 0.99134;
}


void MainWindow::xyz2rgb(double& r, double& g, double& b, double Xx, double Yy, double Zz) {

    r = Xx * 2.04159 + Yy * -0.56501 + Zz * -0.34473;
    g = Xx * -0.96924 + Yy * 1.87597 + Zz * 0.04156;
    b = Xx * 0.01344 + Yy * -0.11836 + Zz * 1.01517;

}

void MainWindow::xyz2lab(double Xx, double Yy, double Zz, double& L, double& a, double& b) {
    // Wartości referencyjne dla bieli D65
    const double Xr =  0.95047;
    const double Yr =  1.00000;
    const double Zr =  1.08883;

    const double epsilon = 0.008856;
    const double kappa = 903.3;

    double xr = Xx / Xr;
    double yr = Yy / Yr;
    double zr = Zz / Zr;

    // labda fubncjkji
    auto f = [=](double t) {
        return (t > epsilon) ? cbrt(t) : ((kappa * t + 16.0) / 116.0);
    };

    double fx = f(xr);
    double fy = f(yr);
    double fz = f(zr);

    // l*a*b* wartosc
    L = (116.0 * fy) - 16.0;
    a = 500.0 * (fx - fy);
    b = 200.0 * (fy - fz);
}


void MainWindow::lab2xyz(double& Xx, double& Yy, double& Zz, double L, double a, double b) {
    // wartości referencyjne dla bieli D65
    const double Xr =  0.95047;
    const double Yr =  1.00000;
    const double Zr =  1.08883;

    const double epsilon = 0.008856;
    const double kappa = 903.3;

    double yr = (L > kappa * epsilon) ? pow((L + 16.0) / 116.0, 3) : L / kappa;
    //
    double fy = (yr > epsilon) ? (L + 16.0) / 116.0 : (kappa*yr + 16) / 116;


    double fx = a / 500.0 + fy;
    double fz = fy - (b / 200.0);

    auto f_inv = [=](double t) {
        return (pow(t, 3) > epsilon) ? pow(t, 3) : (116.0 * t - 16.0) / kappa;
    };

    double xr = f_inv(fx);

    double zr = f_inv(fz);


    Xx = xr * Xr;
    Yy = yr * Yr;
    Zz = zr * Zr;
}

void MainWindow::test() {
    uchar* bits = image.bits();
    uchar* process_bits = image_cpy.bits();

    // wartosc suwakow
    double deltaL = ui->Lslider->value();
    double deltaA = ui->aslider->value();
    double deltaB = ui->bslider->value();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            int index = 4 * (y * image.width() + x);

            // RGB W 0 - 1
            double r = bits[index + 2] / 255.0;
            double g = bits[index + 1] / 255.0;
            double b = bits[index] / 255.0;

            //  RGB → XYZ
            double Xx, Yy, Zz;
            rgb2xyz(r, g, b, Xx, Yy, Zz);

            //  XYZ → LAB
            double L, A, B;
            xyz2lab(Xx, Yy, Zz, L, A, B);

            // + SUWAKI
            L += deltaL;
            A += deltaA;
            B += deltaB;

            // zkresy
            L = clamp(L, 0.0, 100.0);
            A = clamp(A, -128.0, 127.0);
            B = clamp(B, -128.0, 127.0);

            // LAB → XYZ
            lab2xyz(Xx, Yy, Zz, L, A, B);

            //  XYZ → RGB
            xyz2rgb(r, g, b, Xx, Yy, Zz);

            //  przeskalowanie do [0, 255]
            process_bits[index + 2] = clamp(r * 255.0, 0.0, 255.0);
            process_bits[index + 1] = clamp(g * 255.0, 0.0, 255.0);
            process_bits[index]     = clamp(b * 255.0, 0.0, 255.0);
        }
    }
    show_image();
}













