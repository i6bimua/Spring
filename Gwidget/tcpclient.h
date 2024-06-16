#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QFile>
#include <QLabel>
#include <QTcpSocket>
#include <QMainWindow>
#include <QCloseEvent>

namespace Ui
{
    class TcpClient;
}

class TcpClient : public QMainWindow
{
    Q_OBJECT

signals:
    void readyMap(QByteArray Messagereceive);
    void readyMapData(QByteArray Messagereceive);
    void readyTime(int code);

protected:
    void closeEvent(QCloseEvent *event);

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

    //储存排名信息的文件
    QFile *File;

    //接收到的文件
    QFile *FileRcv;

    //文件字节数,已经发送的字节数，剩余的字节数
    qint64 FileBytes, SendBytes, RestBytes;

    //
    QString FileStr;

    //接收到的数据流（字节流）
    QByteArray RcvData;

    //包长度
    int PackLen = 0;

    //接收文件的文件名长度
    int RcvNameLen;


    qint64 RcvFileSize;
    qint64 CurDataLen;
    QString AddIp;

private:
    Ui::TcpClient *ui;

    QTcpSocket  *tcpClient;  //socket
    QLabel  *StateLabel;  //状态栏显示标签

    QString getLocalIP();//获取本机IP地址

    //插入不同字节作为识别码
    void ConFile();
    void AckFile();
    void SendFile();
    void CompFile();

private slots:
    //连接、未连接
    void onConnected();
    void onDisconnected();

    //状态栏的切换，改变状态
    void onChange(QAbstractSocket::SocketState socketState);

    //读取文件内容
    void onReadyRead();

    //处理文件解析文件内容
    void DealFile(QByteArray FileserReceiveper);


    void on_actConnect_triggered();
    void on_actDisconnect_triggered();
    void on_actClear_triggered();
    void on_btnSend_clicked();

    void on_actbackend_triggered();

public:
    //发送排行榜文件
    void sendRank();

    void sendMap(QVector<QVector<int>>& vctCurMap, int code);

    void sendTime(int code);
};

#endif // TCPCLIENT_H
