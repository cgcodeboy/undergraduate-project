#include "httpdownload.h"

HttpDownload::HttpDownload(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);//设置类型
    proxy.setHostName("us03.ipip.pm");//设置代理服务器地址
    proxy.setPort(53544);//设置端口

//    m_manager->setProxy(proxy);
}

void HttpDownload::setDownloadUrl(const QString &url)
{
    m_url = url;
}

void HttpDownload::setStorePath(const QString &path)
{
    m_path = path;
}

void HttpDownload::startDownload()
{
    if(m_url.isEmpty()||m_path.isEmpty()){
        QMessageBox::warning(NULL,"提示信息","url为空或存储地址为空!");
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(m_url));
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);

//    request.setRawHeader("User-Agent", "My app name v0.1");
//    request.setRawHeader("X-Custom-User-Agent", "My app name v0.1");
//    request.setRawHeader("Accept-Encodeing","gzip,deflate,br");
//    request.setRawHeader("Connection","Keep-Alive");
//    request.setRawHeader("Accept","text/html, application/xhtml+xml, image/jxr, */*");

    m_reply = m_manager->get(request);

    if(!m_reply){
        qDebug()<<"reply null";
        return;
    }

    connect(m_reply,SIGNAL(readyRead()),this,SLOT(downloadFinished()));

    connect(m_reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

    connect(m_reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(networkErrorSlot(QNetworkReply::NetworkError)));

    connect(m_reply,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(sslErrorSlot(QList<QSslError>)));

    connect(&timer,SIGNAL(timeout()),this,SLOT(runningSlot()));
    timer.start(1000);
}

void HttpDownload::downloadFinished()
{
    QByteArray replyByte = m_reply->readAll();

    int index = m_url.lastIndexOf('/');
    QString fileName = m_url.right(m_url.length()-index-1);
    QFile file(m_path+"/"+fileName);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::warning(NULL,"提示信息","文件打开失败");
        return;
    }
    file.write(replyByte);
    file.close();
    QMessageBox::information(NULL,"提示信息","下载完毕!");
    m_reply->deleteLater();
}

void HttpDownload::downloadProgress(qint64 sent, qint64 total)
{
    qDebug()<<sent<<" "<<total;
}

void HttpDownload::networkErrorSlot(QNetworkReply::NetworkError error)
{
    qDebug()<<m_url<<" "<<m_reply->errorString();
}

void HttpDownload::sslErrorSlot(QList<QSslError> list)
{
    qDebug()<<"ssl error";
}

void HttpDownload::runningSlot()
{
    qDebug()<<m_reply->isFinished();
}

