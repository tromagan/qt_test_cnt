#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int check_cnt32(QFile *fd);

private slots:
    void on_pushButton_openfile_clicked();

    void on_pushButton_check_cnt32_clicked();

private:
    Ui::MainWindow *ui;
    QString file_name;
    QFile *fd;
    QSettings       my_settings;
};

#endif // MAINWINDOW_H
