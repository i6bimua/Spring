#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QFile>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileDialog>
#include <QMainWindow>

namespace Ui
{
    class TcpServer;
}


class TcpServer : public QMainWindow
{
    Q_OBJECT

signals:
    void readyMap(QByteArray Messagereceive);
    void readyMapData(QByteArray Messagereceive);
    void readyTime(int code);


public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    //排行榜文件
    QFile *File;

    //接收到的文件
    QFile *FileRcv;

    //文件字节数,发送的字节数,剩余的字节数,加载了的字节数
    qint64 FileBytes, SendBytes, RestBytes, loadBytes;

    //文件名
    QString FileStr;

    //接收到的数据流
    QByteArray RcvData;

    //包长度
    int PackLen = 0;

    //文件名长度
    int RcvNameLen;

    //接收到的文件大小
    qint64 RcvFileSize;

    //每一部分的数据大小
    qint64 CurDataLen;
    QString AddIp;

    void sendRank();

    void sendMap(QVector<QVector<int>>& vctCurMap, int code);

    void sendTime(int code);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::TcpServer *ui;
    //状态栏标签
    QLabel  *LabListen;
    QLabel  *StateLabel;

    //TCP服务器
    QTcpServer *tcpServer;

    //TCP通讯的Socket
    QTcpSocket *tcpSocket;

    QString getLocalIP();//获取本机IP地址
    //模拟tcp传输过程并加入验证解析
    void ConFile();
    void AckFile();
    void SendFile();
    void CompFile();

private slots:
    //QTcpServer的newConnection()信号
    void onNewConnection();

    //接收文件
    void onReadyRead();

    //状态栏的改变
    void onChange(QAbstractSocket::SocketState socketState);
    void onClientConnected();
    void onClientDisconnected();

    //对报文进行解析并且处理成文件
    void DealFile(QByteArray FileserReceiveper);

    //槽函数
    void on_actStop_triggered();
    void on_actClear_triggered();
    void on_btnSend_clicked();
    void on_actStart_triggered();
    void on_actbackend_triggered();
};

#endif // TCPSERVER_H
