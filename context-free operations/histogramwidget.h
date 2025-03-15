#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = nullptr);
    void setImage(QImage newImage);

// signals:
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QImage image_hist;
    QVector<int> redHistogram;
    QVector<int> greenHistogram;
    QVector<int> blueHistogram;

    void computeHistogram();
    void drawHistogram(QPainter& painter);
};

#endif // HISTOGRAMWIDGET_H
