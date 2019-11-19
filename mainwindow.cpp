#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QPlainTextEdit"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->plainTextEdit->appendPlainText("123");

    file_name = "";
    //QSettings my_settings();
}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::check_cnt32(QFile *fd)
{
    quint32 words_cnt = 0;
    quint32 ref_word, rd_word;
    quint32 errors_cnt = 0;

    while(!fd->atEnd())
    {
        QByteArray ba = fd->read(4);

        memcpy(&rd_word, ba.constData(), 4);

        if(words_cnt != 0)
        {
            if (rd_word != ref_word)
            {
                qDebug() << "error cnt! Word from file -- Reference word: " + QString::number(rd_word,16) + "  " + QString::number(ref_word,16);
                qDebug() << "Index = " + QString::number(words_cnt,10) + " byte offset " + QString::number(words_cnt << 2,10);
                ui->plainTextEdit->appendPlainText("error cnt! Word from file -- Reference word: " + QString::number(rd_word,16) + "  " + QString::number(ref_word,16));
                ui->plainTextEdit->appendPlainText("Index = " + QString::number(words_cnt,10) + " byte offset " + QString::number(words_cnt << 2,10));
                errors_cnt++;
            }

        }

        ref_word = rd_word + 1;


        words_cnt++;

        if(errors_cnt >= 8)
        {
            qDebug("too much errors! stop checking...");
            ui->plainTextEdit->appendPlainText("too much errors! stop checking...");
            break;
        }
    }

    return words_cnt;
}

void MainWindow::on_pushButton_openfile_clicked()
{
    const QString DEFAULT_DIR_RAW_FILE("last_dir");

    file_name = QFileDialog::getOpenFileName(this, "Open file with binary cnt", my_settings.value(DEFAULT_DIR_RAW_FILE).toString(),"raw (*.pcm *.bin *.dat)");

    if(!file_name.isEmpty())
    {
        my_settings.setValue(DEFAULT_DIR_RAW_FILE, file_name);
        qDebug() << "file is " + file_name;
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText("file is " + file_name);


    }
    else
    {
        qDebug() << "file open error ";
        ui->plainTextEdit->appendPlainText("file open error ");
    }

}

void MainWindow::on_pushButton_check_cnt32_clicked()
{
    if(file_name.isEmpty())
    {
        qDebug() << "file not opened!";
        ui->plainTextEdit->appendPlainText("file not opened!");
        return;
    }



    qDebug() << "start checking 32 bit counter...";
    ui->plainTextEdit->appendPlainText("start checking 32 bit counter...");


    fd = new QFile(file_name);

    if(!fd->open(QIODevice::ReadOnly))
    {
        qDebug() << "Error open file" + file_name;
        ui->plainTextEdit->appendPlainText("Error open file" + file_name);
        fd = nullptr;
        return;
    }
    else
    {
        qDebug() << "File opened " + file_name;
        ui->plainTextEdit->appendPlainText("File opened " + file_name);
    }

    uint32_t words_cnt;
    words_cnt = check_cnt32(fd);

    qDebug() << "test done," + QString::number((words_cnt << 2) >> 10,10) + " KBytes";
    ui->plainTextEdit->appendPlainText("test done," + QString::number((words_cnt << 2) >> 10,10) + " KBytes");

    if(fd != nullptr)
        fd->close();
}
