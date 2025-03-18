#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

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
    void rgb2xyz(double,double,double,double&,double&,double&);
    void xyz2rgb(double&,double&,double&,double,double,double);
    void xyz2lab(double,double,double,double&,double&,double&);
    void lab2xyz(double&,double&,double&,double,double,double);

public slots:
    void open_image();
    void test();

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage image_cpy;
};

#endif // MAINWINDOW_H
