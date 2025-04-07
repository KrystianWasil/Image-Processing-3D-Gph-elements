#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <vector>
#include <complex>
#include <fftw3.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void apply_fft_filter();
    void show_image();
    void resize_image_with_border(int);
    void apply_filter();
    void fft1d(std::vector<std::complex<double>>& data, bool invert);
    void fft2d(std::vector<std::vector<std::complex<double>>>& data, bool invert);
    void test_fft();
    void fft2dUsingFFTW();


public slots:
    void open_image();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage image_cpy;
    std::vector<std::vector<float>> mask;
    int r;
};

#endif // MAINWINDOW_H
