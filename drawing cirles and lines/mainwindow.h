#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QResizeEvent>


// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QImage image;
    QPoint firstPoint;
    QPoint secondPoint;
    bool firstClick = true;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // void show_image();
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
    void drawLine(QPoint p1, QPoint p2);

};

#endif // MAINWINDOW_H
