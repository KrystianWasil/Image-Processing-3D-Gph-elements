#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <histogramwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show_image();
    // uchar processContrast(uchar v, float c);
    // void createGammaTable(float gamma, float* gamma_table);
    void processImage();
    void createLUT(int brightness, int contrast, float gamma);
    // void updateHist();
    void normalize();

public slots:
    void open_image();
    void brightenss(int v);
    void contrast(int c);
    void gamma(int g);
    // void show_hist();

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage image_cpy;

    HistogramWidget* histogramWidget;
    int brightness_value = 0;
    int contrast_value = 0;
    float gamma_value = 1.0f;
    float look_up_table[256];
    // float bright[256];
    // float gamm[256];
    // float contr[256];
    // float histR[256]={0};
    // float histG[256]={0};
    // float histB[256]={0};

    // QChartView* chartViewR;
    // QBarSet *set0;
    // QBarSeries* seriesR;



};

#endif // MAINWINDOW_H
