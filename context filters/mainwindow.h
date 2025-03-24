#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <vector>


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
    void resize_image_with_border(int);
    void apply_filter();
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
