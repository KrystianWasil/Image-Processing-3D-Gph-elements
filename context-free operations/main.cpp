#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
// #include <QApplication>
// #include <QtCharts/QChartView>
// #include <QtCharts/QBarSeries>
// #include <QtCharts/QBarSet>
// #include <QtCharts/QBarCategoryAxis>
// #include <QtCharts/QValueAxis>


//     int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);


//     float pixelCount[256];
//     for (int i=0;i<256;++i) pixelCount[i] = (i+1.3f)/3;


//     QBarSeries *series = new QBarSeries();


//     QBarSet *set = new QBarSet("Liczba pikseli");
//     for (int i = 0; i < 256; ++i) {
//         *set << pixelCount[i];
//     }
//     series->append(set);


//     QChart *chart = new QChart();
//     chart->addSeries(series);
//     chart->setTitle("Histogram wartości pikseli");
//     chart->setAnimationOptions(QChart::SeriesAnimations);


//     QStringList categories;
//     for (int i = 0; i < 256; ++i) {
//         categories << QString::number(i);
//     }
//     QBarCategoryAxis *axisX = new QBarCategoryAxis();
//     axisX->append(categories);
//     chart->addAxis(axisX, Qt::AlignBottom);
//     series->attachAxis(axisX);


//     QValueAxis *axisY = new QValueAxis();
//     axisY->setLabelFormat("%d");
//     axisY->setTitleText("Liczba pikseli");
//     chart->addAxis(axisY, Qt::AlignLeft);
//     series->attachAxis(axisY);


//     QChartView *chartView = new QChartView(chart);
//     chartView->setRenderHint(QPainter::Antialiasing);


//     chartView->resize(800, 600);


//     chartView->show();

//     return a.exec();
// }




