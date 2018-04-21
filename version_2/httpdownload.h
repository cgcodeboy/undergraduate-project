#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QTimer>
#include <QSslConfiguration>
#include <QSsl>

class HttpDownload : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownload(QObject *parent = 0);

    void setDownloadUrl(const QString &url);

    void setStorePath(const QString &path);

    void startDownload();

signals:

private slots:
    void downloadFinished();
    void downloadProgress(qint64 sent,qint64 total);
    void networkErrorSlot(QNetworkReply::NetworkError error);
    void sslErrorSlot(QList<QSslError> list);
    void runningSlot();

private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QString m_url;
    QString m_path;
    QTimer timer;
};

#endif // HTTPDOWNLOAD_H
