#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->BMap_webView->setUrl(QUrl("qrc:/page/map.html"));

    ui->BMap_webView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_show_BT_clicked()
{
    double B = 0, L = 0;
    double x = 0, y = 0;
    int i = 0;

    QFile file("current_20140717.txt");

    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open file error!";
        return;
    }
    QWebFrame *webFrame = ui->BMap_webView->page()->mainFrame();
    while(!file.atEnd()){
        QByteArray line =  file.readLine();
        QString str(line);
        if(str.length() > 20&&i <20){
            QStringList strList = str.split(' ');
            qDebug()<<strList.at(1)<<" "<<strList.at(2);
            QString lat = strList.at(1);
            QString lon  = strList.at(2);
            QString cmd = QString("showAddress(\"%1\",\"%2\")").arg(lat).arg(lon);
            webFrame->evaluateJavaScript(cmd);
            i++;
        }
    }
}
