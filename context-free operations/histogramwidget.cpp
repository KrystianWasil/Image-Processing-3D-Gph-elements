#include "histogramwidget.h"
#include <QPainter>

HistogramWidget::HistogramWidget(QWidget *parent)
    : QWidget{parent}
{

    redHistogram.resize(256, 0);
    greenHistogram.resize(256, 0);
    blueHistogram.resize(256, 0);

}

void HistogramWidget::setImage(QImage newImage) {
    image_hist = newImage;
    computeHistogram();
    update();
}

void HistogramWidget::computeHistogram() {
    //resetowanie hsitogramow
    redHistogram.fill(0);
    greenHistogram.fill(0);
    blueHistogram.fill(0);


    for (int y = 0; y < image_hist.height(); ++y) {
        for (int x = 0; x < image_hist.width(); ++x) {
            int index = 4 * (y * image_hist.width() + x);
            redHistogram[image_hist.bits()[index + 2]]++;
            greenHistogram[image_hist.bits()[index + 1]]++;
            blueHistogram[image_hist.bits()[index]]++;
        }
    }
}

void HistogramWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawHistogram(painter);
}

void HistogramWidget::drawHistogram(QPainter& painter) {
    int width = this->width();
    int height = this->height();


    int maxCount = *std::max_element(redHistogram.begin(), redHistogram.end());
    maxCount = std::max(maxCount, *std::max_element(greenHistogram.begin(), greenHistogram.end()));
    maxCount = std::max(maxCount, *std::max_element(blueHistogram.begin(), blueHistogram.end()));

    for (int i = 0; i < 256; ++i) {

        int redHeight = (redHistogram[i] * height) / maxCount;
        painter.setPen(Qt::red);
        painter.drawLine(i, height, i, height - redHeight);

        int greenHeight = (greenHistogram[i] * height) / maxCount;
        painter.setPen(Qt::green);
        painter.drawLine(i + 256, height, i + 256, height - greenHeight);

        int blueHeight = (blueHistogram[i] * height) / maxCount;
        painter.setPen(Qt::blue);
        painter.drawLine(i + 512, height, i + 512, height - blueHeight);
    }
}
