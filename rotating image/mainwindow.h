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

public slots:
    void open_image();
    void skalowanie(int);
    void obrocWzgledemSrodka(int);

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage image_cpy;
};

#endif // MAINWINDOW_H
